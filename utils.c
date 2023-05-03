#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <math.h>

#include "utils.h"
#include "error.h"

/**
 * Function that returns a virtual address space address with the provided size.
 */ 
extern void* mmalloc(size_t size){
   void* mapMemory;
   mapMemory=mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE| MAP_ANONYMOUS, -1, 0);

   if(mapMemory == MAP_FAILED)
    ERROR("Mmap failed!");

   return mapMemory;

}
/**
 * Function that obtains e's size representation.
 */ 
extern unsigned int e2size(int e){
    return 1 << e;
}

/**
 * Function that gets e's size representation.
 */ 
extern int size2e(unsigned int size){
    return ceil(log(size)/log(2));
}

/**
 * Function that sets the bit
 */ 
extern void bitset(unsigned char *p, int bit){
    unsigned char mask = 1 << bit;
    *p |= mask;
}

/**
 * Function that clears the bit
 */
extern void bitclr(unsigned char *p, int bit){
    unsigned char mask = ~(1 << bit);
    *p &= mask;

}

/**
 * Function that inverts the bit
 */
extern void bitinv(unsigned char *p, int bit){
    unsigned char mask = 1 << bit;
    *p^=mask;

}

/**
 * Function that tests the bit
 */
extern int bittst(unsigned char *p, int bit){
    return (*p >> bit) & (1);
}

/**
 * Function that sets the bit to retrieve buddy
 */
extern void *buddyset(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = 1 << e;
    memoryInt|=mask;
    return base + memoryInt;

}

/**
 * Function that clears the bit to retrieve buddy
 */
extern void *buddyclr(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = ~(1 << e);
    memoryInt&=mask;
    return base + memoryInt;
}

/**
 * Function that inverts the bit to retrieve both the left and right buddy
 */
extern void *buddyinv(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    unsigned long mask = 1 << e;
    memoryInt^=mask;
    return base + memoryInt;

}

/**
 * Function that tests the bit of buddy
 */
extern int buddytst(void *base, void *mem, int e){
    unsigned long memoryInt = (unsigned long)mem - (unsigned long)base;
    memoryInt = (memoryInt >> e) & 1;
    return memoryInt;
}