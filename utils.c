#include <stddef.h>
#include <sys/mman.h>
#include "utils.h"
#include <math.h>

//Wrapper around mmap that takes the same parameters as malloc(), just without using malloc itself
extern void *mmalloc(size_t size){
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
}

//exponent to size
//Returns 2^e
extern unsigned int e2size(int e){
    return pow(2.0, (double)e); 
}
//returns log base 2 of size
extern int size2e(unsigned int size){
    return log2(size);
}

extern void bitset(unsigned char *p, int bit){
    *p = bit;
}
extern void bitclr(unsigned char *p, int bit){
    *p &= 0; // 1 & 0   0 & 0
}
extern void bitinv(unsigned char *p, int bit){
    *p = ~*p;
}
extern int bittst(unsigned char *p, int bit){
    return *p == bit;
}


extern void *buddyset(void *base, void *mem, int e){
    return NULL;
}

extern void *buddyclr(void *base, void *mem, int e){
    //WHAT IS THE E IN ALL OF THESE FUNCTIONS USED FOR
    //IT CAN'T BE FOR BITMAPS SINCE THERE'S NO BITMAP HERE

    //HOW MANY BITMAPS DO WE NEED. ALL OF THE WHITEPAPERS SAID ONE PER LEVEL BUT WITH THE E IT SEEMS LIKE IT'S JUST 1 BIG ONE

    //WHAT THE HECK IS THE FREELIST
    return NULL;
}

//The e passed in is left shifted by e in order to find the left buddy of the pair
extern void *buddyinv(void *base, void *mem, int e){
    return NULL;
}


extern int buddytst(void *base, void *mem, int e){
    return 0;
}