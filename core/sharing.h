/*****************************************************************************************[sharing.h]
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

namespace Minisat{
  
#define EXPORTED_LIMIT 10000
#define CLAUSE_LIMIT 8

  class clauseSharing{  
    
  public:
    
    int nProcs;

    vec<Lit> **learntClauses;
    vec<Lit>  *learntUnits;

    int** lastReadC;
    int*  lastWriteC;

    int** lastReadU;
    int*  lastWriteU;


    clauseSharing (){}
    
    void init (int N, int nVars){
      
      nProcs        = N;
      learntClauses = new vec<Lit> *[nProcs];
      learntUnits   = new vec<Lit>  [nProcs];
      lastReadC     = new int*      [nProcs];
      lastReadU     = new int*      [nProcs];
      lastWriteC    = new int       [nProcs];
      lastWriteU    = new int       [nProcs];
      
      for(int i = 0; i < nProcs; i++){

        learntClauses[i] = new vec<Lit> [EXPORTED_LIMIT];
        lastReadC    [i] = new int      [nProcs];
        lastReadU    [i] = new int      [nProcs];
        
        for(int j = 0; j < nProcs; j++){
          
          lastReadC    [i][j] = 0;
          lastReadU    [i][j] = 0;
          lastWriteC      [j] = 0;
          lastWriteU      [j] = 0;
        }
        
        learntUnits[i].capacity(nVars);
      }
    }

    ~clauseSharing(){
      
      for (int i=0; i<nProcs; i++){
        delete [] lastReadC[i];
        delete [] lastReadU[i];
        delete [] learntClauses[i];
      }

      delete [] lastReadC;
      delete [] lastReadU;
      delete [] lastWriteC;
      delete [] lastWriteU;
      delete [] learntClauses;
      delete [] learntUnits;

    }
    
    void    propagateUnitLits      (Solver *S);
    void    exportLearntUnits      (Solver *S);
    void    exportLearntClause     (vec<Lit>& learnt, Solver *S);
    void    importLearntUnits      (Solver *S);
    void    importLearntClauses    (Solver *S);
    void    pushImportedClause     (vec<Lit>& lt, Solver *S);
    
  };

}