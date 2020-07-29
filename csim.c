#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "csim.h"
#include "YOURCODEHERE.h"

cache * initcache(unsigned int size, unsigned int assoc, unsigned int blocksize, char* name, Bool ismem){
  cache * acache = (cache*)malloc(sizeof(cache));
  unsigned int namelen=strlen(name);
  acache->name=(char*)malloc((namelen+1)*sizeof(char));
  strncpy(acache->name,name,namelen);
  acache->name[namelen]=0;
  acache->ismainmemory=ismem;
  acache->nextcache=NULL; // connect externally
  // init all stats to zero
  acache->readhits=0;
  acache->writehits=0;
  acache->readmisses=0;
  acache->writemisses=0;
  acache->evictions=0;
  acache->writebacks=0;

  if(ismem){ // no cache internals for "memory"
    return acache;
  }

  setSizesOffsetsAndMaskFields(acache, size, assoc, blocksize);

  unsigned int curset;
  unsigned int curblock;
  // allocate memory for cache structures
  acache->sets=(cacheset*)malloc(acache->numsets*sizeof(cacheset));
  for(curset=0;curset<acache->numsets;++curset){
    (acache->sets[curset]).numways=assoc;
    (acache->sets[curset]).timestamp=0;
    (acache->sets[curset]).blocks=(cacheblock*)malloc(assoc*sizeof(cacheblock));
    for(curblock=0;curblock<assoc;++curblock){
      acache->sets[curset].blocks[curblock].valid=0;
      acache->sets[curset].blocks[curblock].dirty=0;
      acache->sets[curset].blocks[curblock].tag=0;
      acache->sets[curset].blocks[curblock].policydata=0;
      acache->sets[curset].blocks[curblock].datawords=(unsigned long long*)malloc(blocksize);
	}
  }

  return acache;
}

void linkcache(cache* current, cache* next){
  current->nextcache=next;
}

// Read a word through cache system - wrapper for performaccesse
uint64_t LoadWord(cache* acache, unsigned long long address){
  return performaccess(acache, address, 8, 0, 0, 0);
}

// Write a word through cache system - wrapper for performaccesse
void StoreWord(cache* acache, unsigned long long address,  unsigned long long value){
  performaccess(acache, address, 8, 1, value, 0);
}

// perform cache accesses (at word granularity - block accesses done as sequences)
uint64_t performaccess(cache* acache, unsigned long long address, unsigned int size, Bool write, unsigned long long value, Bool blockAccess){
  unsigned int oldestway=0;
  if(!acache){
    fprintf(stderr,"ERROR: Last level of hierarchy is not a main memory!\n");
    exit(-1); 
  }
  if(acache->ismainmemory){
    if(write){
      if(!blockAccess){acache->writehits++;}
      {
        //INCREDIBLY NON-PORTABLE - DO NOT TRY THIS AT HOME 
        *((unsigned long long*)address)=value;
      }
    } else {
      if(!blockAccess){acache->readhits++;}
    }
    //INCREDIBLY NON-PORTABLE - DO NOT TRY THIS AT HOME 
    return *((unsigned long long*)address);
  } else {
    unsigned long long index = getindex(acache, address);
    unsigned long long tag = gettag(acache, address);
    unsigned int curway;
    unsigned long long oldesttime=acache->sets[index].blocks[0].policydata;
    for(curway=0;curway<acache->numways;++curway){
      if(acache->sets[index].blocks[curway].valid & (acache->sets[index].blocks[curway].tag==tag)){
	acache->sets[index].blocks[curway].policydata = ++(acache->sets[index].timestamp);
	if(write){
	  if(!blockAccess){acache->writehits++;}
	  acache->sets[index].blocks[curway].dirty=1;
          {
            //INCREDIBLY NON-PORTABLE - DO NOT TRY THIS AT HOME 
            acache->sets[index].blocks[curway].datawords[(address&((unsigned long long)(acache->blocksize)-1))>>3]=value;
          }
	} else {
	  if(!blockAccess){acache->readhits++;}
	}
	return acache->sets[index].blocks[curway].datawords[(address&((unsigned long long)(acache->blocksize)-1))>>3];
      } else {
	if(!acache->sets[index].blocks[curway].valid || acache->sets[index].blocks[curway].policydata < oldesttime){
	  oldesttime=acache->sets[index].blocks[curway].policydata;
	  oldestway=curway;
	}
      }
    } 
    // miss handling
    unsigned int curword=0;
    if(acache->sets[index].blocks[oldestway].valid) {
      acache->evictions++;
      
      if(acache->sets[index].blocks[oldestway].dirty){
        acache->writebacks++;
        // perform writeback to lower cache
        writeback(acache, index, oldestway);
      }
    }
    unsigned long long uppervalue =  // handle miss in other layers
    acache->sets[index].blocks[oldestway].valid = 1;
    acache->sets[index].blocks[oldestway].dirty = write;
    acache->sets[index].blocks[oldestway].tag = tag;
    acache->sets[index].blocks[oldestway].policydata = ++(acache->sets[index].timestamp);

    // Fill from upper cache
    fill(acache, index, oldestway, address);

    if(write){ // update newly filled line with written data
      acache->writemisses++;
      acache->sets[index].blocks[oldestway].datawords[(address&((unsigned long long)(acache->blocksize)-1))>>3]=value;
    }else{ // just update stats - line is clean
      acache->readmisses++;
    }

    return acache->sets[index].blocks[oldestway].datawords[(address&((unsigned long long)(acache->blocksize)-1))>>3]; // return load-relevant value  
  }
}

void printstatistics(cache* acache){
  if(!acache){
    return;
  }
  fprintf(stdout,"\nStatistics for %s %s:\n",(acache->ismainmemory)?"memory":"cache",acache->name);
  fprintf(stdout,"\tRead Hits\t%llu\n",acache->readhits);
  fprintf(stdout,"\tWrite Hits\t%llu\n",acache->writehits);
  if(!(acache->ismainmemory)){
    fprintf(stdout,"\tRead Misses\t%llu\n",acache->readmisses);
    fprintf(stdout,"\tWrite Misses\t%llu\n",acache->writemisses);
    fprintf(stdout,"\tEvictions\t%llu\n",acache->evictions);
    fprintf(stdout,"\tWritebacks\t%llu\n",acache->writebacks);
    unsigned long long totalacc=acache->readhits+acache->readmisses+acache->writehits+acache->writemisses;
    unsigned long long totalmiss=acache->readmisses+acache->writemisses;
    fprintf(stdout,"\n\tTotal Access Count\t%llu\n",totalacc);
    fprintf(stdout,"\tLocal Miss Rate\t%3.4g%c\n",((double)100*totalmiss)/totalacc,'%');
  }
  fprintf(stdout,"\n");
  printstatistics(acache->nextcache);
}


void printcachecontents (cache* acache){
  if(!acache){
    return;
  }
  if(acache->ismainmemory){
    fprintf(stdout,"\nMain Memory = { ... }\n");
    return;
  }
  fprintf(stdout,"\nContents of cache %s:\n",acache->name);
  unsigned int curset=0;
  unsigned int curblock=0;
  unsigned int curword=0;
  for(curset=0;curset<acache->numsets;++curset){
    fprintf(stdout,"%8x [ ",curset); 
    for(curblock=0;curblock<acache->numways;++curblock){
      acache->sets[curset].blocks[curblock].policydata=0;
      fprintf(stdout,"{v:%u d:%u tag:%16llx data:|",acache->sets[curset].blocks[curblock].valid,acache->sets[curset].blocks[curblock].dirty,acache->sets[curset].blocks[curblock].tag);
      for(curword=0;curword < ((acache->blocksize)>>3) ;curword++){
        fprintf(stdout,"%16llx|",acache->sets[curset].blocks[curblock].datawords[curword]);
      }
      fprintf(stdout,"} ");
    }
    fprintf(stdout,"]\n");
  }
  
  printcachecontents(acache->nextcache);
}
