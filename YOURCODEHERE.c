#include "YOURCODEHERE.h"

void  setSizesOffsetsAndMaskFields(cache* acache, unsigned int size, unsigned int assoc, unsigned int blocksize){
  unsigned int localVAbits=8*sizeof(uint64_t*);
  if (localVAbits!=64){
    fprintf(stderr,"Running non-portable code on unsupported platform, terminating. Please use designated machines.\n");
    exit(-1);
  }

  // YOUR CODE GOES HERE

}


unsigned long long getindex(cache* acache, unsigned long long address){
  return 0; //Replace with correct value
}

unsigned long long gettag(cache* acache, unsigned long long address){
  return 0; //Replace with correct value
}

void writeback(cache* acache, unsigned int index, unsigned int oldestway){

  // YOUR CODE GOES HERE

}

void fill(cache * acache, unsigned int index, unsigned int oldestway, unsigned long long address){

  // YOUR CODE GOES HERE

}
