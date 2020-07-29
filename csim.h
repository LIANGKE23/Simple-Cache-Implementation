#ifndef __CSIM_H__
#define __CSIM_H__
#include <stdint.h>
typedef unsigned char Bool;

typedef struct cacheblock{
  Bool valid;
  Bool dirty;
  unsigned long long tag;
  unsigned long long policydata;
  unsigned long long * datawords; 
} cacheblock;

typedef struct cacheset{
  unsigned int numways;
  unsigned long long timestamp;
  cacheblock * blocks;
} cacheset;

typedef struct cache{
  unsigned long long readhits;
  unsigned long long writehits;
  unsigned long long readmisses;
  unsigned long long writemisses;
  unsigned long long evictions;
  unsigned long long writebacks;
  Bool ismainmemory;
  unsigned int numsets;
  unsigned int numways;
  unsigned int blocksize;
  cacheset * sets;
  char * name;
  unsigned int BO;
  unsigned int TO;
  unsigned long long VATmask;
  unsigned long long VAImask;
  struct cache * nextcache;
} cache;



// accesses cache - all caches inclusive, write-back, write-allocate with LRU policy
uint64_t performaccess(cache* acache, unsigned long long address, unsigned int size, Bool write, unsigned long long value, Bool blockAccess);

// Read a word through cache system - wrapper for performaccesse
uint64_t LoadWord(cache* acache, unsigned long long address);

// Write a word through cache system - wrapper for performaccesse
void StoreWord(cache* acache, unsigned long long address,  unsigned long long value);

// print hit/miss/access statistics 
void printstatistics(cache* acache);

// print cache meta-data 
void printcachecontents (cache* acache);

// Set initial cache state
cache * initcache(unsigned int size, unsigned int assoc, unsigned int blocksize, char* name, Bool ismem); 

// Create hierarchy from individual caches
void linkcache(cache* current, cache* next);
#endif
