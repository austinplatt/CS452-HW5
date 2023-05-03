#include <string.h>

#include "balloc.h"

// Static variable to store the base pointer of the pool.
static Balloc bp=0;

/**
 * Implements the standard C library function malloc using balloc.
 */ 
extern void *malloc(size_t size) {
  //If the base pointer is NULL, create a new pool with default size and limits.
  bp=bp ? bp : bnew(16384,4,12);
  //Allocate memory from the pool using balloc and return the pointer.
  return balloc(bp,size);
}

/**
 * Implements the standard C library function free using bfree.
 */ 
extern void free(void *ptr) {
  //Free the memory pointed to by ptr using bfree.
  bfree(bp,ptr);
}
//Implements the min function to get the minimum of two sizes.
size_t min(size_t x, size_t y) { return x<y ? x : y; } 
extern void *realloc(void *ptr, size_t size) {
  //Allocate memory from the pool using malloc.
  void *new=malloc(size);
  // If ptr is NULL, return the new pointer.
  if (!ptr) 
    return new;
  //Copy the contents of the old memory block to the new block up to the minimum size.
  memcpy(new,ptr,min(size,bsize(bp,ptr)));
  //Free the old memory block using free.
  free(ptr); 
  //Return the new memory block.
  return new; 
}
