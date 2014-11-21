/*****************************************************************************************[parallel.h]
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


#ifndef PARALLEL_H
#define PARALLEL_H

#include<pthread.h>
#include<stdio.h>

class parallel_data
{
private:

  bool exitFlag;
  pthread_mutex_t exitFlagMux;
  pthread_barrier_t syncBarrier;  

  
  inline bool getExitFlag(){
    
      return exitFlag;
  }  

public:
  
  void exitThread(){
    
    syncThreads();
    pthread_exit( (void*) 0);      
  }
  
  void setExitFlag(bool value){

      pthread_mutex_lock(&exitFlagMux);
      
      if (getExitFlag() == true){
        pthread_mutex_unlock(&exitFlagMux);
        exitThread();
      }

      exitFlag=value;
      
      pthread_mutex_unlock(&exitFlagMux);
  }

  void initStructures(int nProcs){
    
      exitFlag = false;
      pthread_mutex_init(&exitFlagMux, NULL);
      pthread_barrier_init(&syncBarrier, NULL, nProcs); 

  }

  inline void checkOtherThreads(){
    
      if(getExitFlag() == true) exitThread();
  }
  
  
  void syncThreads(void){
    
    pthread_barrier_wait(&syncBarrier);
  }

};

#endif