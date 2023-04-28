#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>

#include "utils.h"
#include "error.h"

//<summary>Returns an address in the virtual address space of specified size</summary>
//<param>Size: The size of memory to return</param>
//<returns>address of the virtual memory</returns>
extern void* mmalloc(size_t size){
   void* mapMemory;
   mapMemory=mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE| MAP_ANONYMOUS, -1, 0);

   if(mapMemory == MAP_FAILED)
    ERROR("Mmap failed!");

   return mapMemory;

}

// Gets the size representation for e
extern unsigned int e2size(int e){
    return 1 << e;
}

// Gets the e representation for size
extern int size2e(unsigned int size){
    return ceil(log(size)/log(2));
}

// Sets a bit
extern void bitset(unsigned char *p, int bit){
    unsigned char mask = 1 << bit;
    *p |= mask;
}

// Clears a bit
extern void bitclr(unsigned char *p, int bit){
    unsigned char mask = ~(1 << bit);
    *p &= mask;

}

// Inverts a bit
extern void bitinv(unsigned char *p, int bit){
    unsigned char mask = 1 << bit;
    *p^=mask;

}

// Tests a bit
extern int bittst(unsigned char *p, int bit){
    return (*p >> bit) & (1);
}

// Sets the bit to retrieve buddy
extern void *buddyset(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = 1 << e;
    memoryInt|=mask;
    return base + memoryInt;

}

// clears a bit to retrieve buddy
extern void *buddyclr(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = ~(1 << e);
    memoryInt&=mask;
    return base + memoryInt;
}

// Inverts a bit to retrive left/right buddy
extern void *buddyinv(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = 1 << e;
    memoryInt^=mask;
    return base + memoryInt;

}

// Tests bit of a buddy
extern int buddytst(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    memoryInt = (memoryInt >> e) & 1;
    return memoryInt;
}