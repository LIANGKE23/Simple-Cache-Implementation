#include <stdint.h>

// command line values for main
typedef struct initialization_struct {
  unsigned int N;
  unsigned int iterations;
  uint64_t * allMatrices;
} init_struct ; 


// perform initialization
init_struct init (int argc, char** argv); 

// computes dense, naive O(N^3) matrix multiply C=AB for NxN matrices of uint64_ts A,B,C
void NaiveDDPMatMul(unsigned int N, uint64_t* C, uint64_t* A, uint64_t* B); 
