#include <string.h>

#include "balloc.h"

static Balloc bp=0;

extern void *malloc(size_t size) {
  bp=bp ? bp : bnew(16384,4,12);
  return balloc(bp,size);
}

extern void free(void *ptr) {
  bfree(bp,ptr);
}

size_t min(size_t x, size_t y) { return x<y ? x : y; } //Returns the minimum size of 2 values. Moved it outside the realloc because it was causing issues
extern void *realloc(void *ptr, size_t size) {
  void *new=malloc(size);
  if (!ptr) //If the ptr is NULL, return the new memory like malloc
    return new;
  memcpy(new,ptr,min(size,bsize(bp,ptr)));//Otherwise copy the memory from the ptr into the new memory
  free(ptr); //Free the original pointer
  return new; //Return the new memory
}
