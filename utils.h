#include <stdint.h>

 // prints a square NxN matrix of uint64_ts
void printsquarematrix(unsigned int N, uint64_t* ULC);

// fills a memory array of uint64_t of length "bound" with LFSR pattern
void patternFill(uint64_t * ptr, unsigned int bound);
