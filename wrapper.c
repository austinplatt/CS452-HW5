#include <string.h>
#include "balloc.h"

static Balloc bp = 0;

extern void* malloc(size_t size) {
  bp = bp ? bp : bnew(4096, 4, 10);
  return balloc(bp, size);
}

extern void free(void* ptr) {
  bfree(bp, ptr);
}

extern void* realloc(void* ptr, size_t size) {
  size_t min(size_t x, size_t y) { return x<y ? x : y; }
  void* new_ptr = malloc(size);
  if (!ptr)
    return new_ptr;
  memcpy(new_ptr, ptr, min(size, bsize(bp, ptr)));
  free(ptr);
  return new_ptr;
}
