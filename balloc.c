#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>

#include "utils.h"
#include "freelist.h"
#include "bitmap.h"
#include "error.h"
#include "balloc.h"

typedef struct {
  void* baseAddr;
  int upper;
  int lower;
  unsigned int size;
  FreeList list;
} *Rep;

/**
 * Creates a new Balloc instance with the specified size, lower bound, and upper bound.
 * If the lower bound is less than 3, it is set to 3.
 * Calculates the element size and number of elements to allocate,
 * allocates the required memory using mmalloc(), and creates a new Rep object to hold the metadata.
 */
extern Balloc bnew(unsigned int size, int l, int u){
    l= l < 3 ? 3 : l;
    unsigned int eSize = e2size(size2e(size));
    unsigned int e = size2e(eSize);
    void* mapMemory=mmalloc(eSize);

    Rep r = (Rep) mmalloc(sizeof(*r));
    r->baseAddr=mapMemory;
    r->upper=u;
    r->size= size;
    r->list = freelistnew(size, r->baseAddr, l, u);
    r->lower=l;
    return (Balloc)r;
}


/**
 * Allocate a block of memory of size 'size' from the memory pool 'pool'.
 * If successful, return a pointer to the allocated block.
 * Otherwise, return NULL.
 */
extern void *balloc(Balloc pool, unsigned int size){
    unsigned int eSize = e2size(size2e(size));
    unsigned int e = size2e(eSize);

    Rep r = (Rep)pool;
    int currMem = r->baseAddr;
    void* bAddress = NULL;
    e = e < r->lower ? r->lower : e;
    bAddress = freelistalloc(r->list, r->baseAddr, e, r->lower, r->upper);
    return bAddress;
}
/**
 * Frees a memory block allocated from a given Balloc pool.
 */
extern void bfree(Balloc pool, void *mem){
   Rep r = (Rep)pool;

   int size = bsize(pool,mem);
   int e = size2e(size);

   freelistfree(r->list, r->baseAddr, mem, e, r->lower);
}
/**
 * This function takes a memory pool and a pointer to an allocated memory
 * block in the pool, and returns the size of the memory block in bytes.
 */ 
extern unsigned int bsize(Balloc pool, void *mem){
  Rep r = (Rep)pool;
  int size = freelistsize(r->list, r->baseAddr, mem, r->lower, r->upper);
  return size;
}
/**
 * Print the details of a Balloc pool.
 */ 
extern void bprint(Balloc pool){
   Rep r = (Rep)pool;
   printf("Base: %p\n Low: %d\n High: %d\n", r->baseAddr, r->lower, r->upper);
   freelistprint(r->list, r->size, r->lower, r->upper);
}