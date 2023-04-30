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

// Initializes our allocator instance
// Should hold reference to freelist,
// and upper and lower values so we can query the fList.
// Should also hold th
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

// Uses allocater to allocate a block
// returns address of block to user
extern void *balloc(Balloc pool, unsigned int size){
    unsigned int eSize = e2size(size2e(size));
    unsigned int e = size2e(eSize);

    Rep r = (Rep)pool;
    int currMem = r->baseAddr;
    void* bAddress = NULL;
    // Calculates requested e
    e = e < r->lower ? r->lower : e;
    bAddress = freelistalloc(r->list, r->baseAddr, e, r->lower, r->upper);
    return bAddress;
}

// Frees a block and adds back to freelist
extern void bfree(Balloc pool, void *mem){
   Rep r = (Rep)pool;

   int size = bsize(pool,mem);
   int e = size2e(size);

   freelistfree(r->list, r->baseAddr, mem, e, r->lower);
}
extern unsigned int bsize(Balloc pool, void *mem){
  Rep r = (Rep)pool;
  int size = freelistsize(r->list, r->baseAddr, mem, r->lower, r->upper);
  return size;
}

extern void bprint(Balloc pool){
   Rep r = (Rep)pool;
   printf("Base: %p\n Low: %d\n High: %d\n", r->baseAddr, r->lower, r->upper);
   freelistprint(r->list, r->size, r->lower, r->upper);
}