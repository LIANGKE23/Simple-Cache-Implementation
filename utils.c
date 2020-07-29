#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void printsquarematrix(unsigned int N, uint64_t* ULC){
  unsigned int i, j;
  for(i=0;i<N;++i){
    if(0==i){
      fprintf(stdout,"[ [");
    } else {
      fprintf(stdout,"\n  [");
    }
    for(j=0;j<N;++j){
      fprintf(stdout,"\t%16llx",ULC[i*N+j]);
    }
    fprintf(stdout," ]");
  }
  fprintf(stdout," ]\n");
}

void patternFill(uint64_t * ptr, unsigned int bound){
  uint64_t base=0xFEDCA9876543210;
  unsigned int iter=0;
  // initialize all matrix values
  for(;iter<bound;++iter){
    ptr[iter]=(base%16)+1;
    uint64_t tmp = ( (base)^ (base>>2) ^ (base>>3) ^ (base>>5) )  & 1;
    base = ( base >> 1) | ( tmp << 63 );
  }
}
 
