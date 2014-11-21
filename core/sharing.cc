/*****************************************************************************************[sharing.cc]
Copyright (c) 2012-2013, Ricardo de Sousa Marques

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include "mtl/Sort.h"
#include "core/Solver.h"
#include "core/sharing.h"

using namespace Minisat;

void clauseSharing::propagateUnitLits(Solver *S){

  for(int i = 0; i < S->unitLits.size(); i++){

    if(S->value(S->unitLits[i]) == l_Undef){
      S->uncheckedEnqueue(S->unitLits[i]);
    }
  }

  S->unitIndex = S->trail.size();
  S->unitLits.clear();
  
}


void clauseSharing::exportLearntUnits(Solver *S){
  
  int tid = S->thread_id;
  
  assert(S->decisionLevel() == 0);
  
  for (int i=S->unitIndex; i<S->trail.size(); i++){
    learntUnits[tid].push(S->trail[i]);
  }
  
  lastWriteU[tid] = learntUnits[tid].size();
  S->unitIndex    = S->trail.size();
          
}

void clauseSharing::exportLearntClause(vec<Lit>& learnt, Solver *S){
 
  int tid = S->thread_id;
  
  if (S->decisionLevel() == 0) {
    exportLearntUnits(S);
  }
    
  else if (learnt.size() < CLAUSE_LIMIT){
    
    learnt.copyTo(learntClauses[tid][lastWriteC[tid]]);
    lastWriteC[tid] = lastWriteC[tid] == (EXPORTED_LIMIT - 1) ? 0 : lastWriteC[tid]+1;
    
  }
}

void clauseSharing::importLearntUnits(Solver *S){
    
  int tid = S->thread_id;
  
  for (int i=0; i<nProcs; i++){
    
    if (i == tid) continue;
    
    int lastWrite = lastWriteU[i];
     
    for (int j=lastReadU[i][tid]; j<lastWrite; j++){      
      S->unitLits.push(learntUnits[i][j]);
    }
    
    lastReadU[i][tid] = lastWrite;
    
  }
}

void clauseSharing::importLearntClauses(Solver *S){
  
  int tid = S->thread_id;
  
  for(int i = 0; i < nProcs; i++){
    
    if(i == tid) continue;
    
    int first = lastReadC[i][tid];
    int last  = lastWriteC[i];
    
    if(first == last) continue;
    
    int end = last < first ? EXPORTED_LIMIT : last; 
    
    for(int j = first; j < end; j++) pushImportedClause(learntClauses[i][j], S);
    
    if (last < first) for(int j = 0; j < last; j++) pushImportedClause(learntClauses[i][j], S);

    lastReadC[i][tid] = (last == EXPORTED_LIMIT) ? 0 : last;
  }
}    

void clauseSharing::pushImportedClause(vec<Lit>& lt, Solver *S){
  
  CRef cr = S->ca.alloc(lt, true);

  S->learnts.push(cr);
  S->attachClause(cr);
  S->claBumpActivity(S->ca[cr]);

}
