#include "bitmap.h"
#include "utils.h"
#include "freelist.h"
#include "balloc.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

typedef struct
{
    FreeList free_list;
    BitMap bit_map;
} FreeArea;

typedef struct
{
    void *baseAddress;
    unsigned int poolSize;
    int lowerE;
    int upperE;
    FreeArea **free_area;
} BallocRep;

//size is a number, if that number isn't on a 2^x boundary, then jump to the next boundary
//lowerBound, the x in 2^x. Lower bound of size of memory allowed to be allocated
//upperBound, same as above, just maximum size that someone can alloc at one time
//L's lowest value should be 3
extern Balloc bnew(unsigned int size, int l, int u)
{
    int *base;
    unsigned int realSize;
    int isPowerOf2 = 1;           //Assume true because we can only prove it's not a power of 2 by contradiction
    unsigned int testSize = size; //Don't mess with size because we'll need it later
    while (testSize != 1)
    { //127 128 129 130
        if (testSize % 2 != 0)
        {
            isPowerOf2 = 0;
            break;
        }
        testSize /= 2;
    }

    if (isPowerOf2)
    {
        realSize = size;
    }
    else
    {
        //If not a power of 2, find the next largest one
        unsigned int temp = 1;
        while (temp < size)
        {
            temp *= 2;
        }
        realSize = temp;
    }
    //NULL means that the kernel decides on an address for us to go to
    //Base is the address of the memory the Kernel gives us
    base = mmap(NULL, realSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (base == MAP_FAILED)
    {
        ERROR("Failed to mmap");
    }
    Balloc b = (Balloc *)mmalloc(sizeof(Balloc)); //mmalloc because we aren't allowed to use real malloc
    if (b == MAP_FAILED)
    {
        ERROR("Failed to allocate memory for Balloc struct");
    }
    BallocRep *bRep = (BallocRep *)b;
    bRep->baseAddress = base;
    bRep->poolSize = realSize;

    int realLower = l < 3 ? 3 : l; //Ensure that l is at least 3 since pointers are minimum 8 bytes (following the instructions of buff)
    bRep->lowerE = realLower;
    bRep->upperE = u;

    //Start on the free area mmallocs
    bRep->free_area = (FreeArea**)mmalloc(sizeof(FreeArea *) * (u - realLower)+1);
    if(bRep->free_area == MAP_FAILED)
        ERROR("Failed to allocate memory for FreeArea Array");
    //Give the free_areas some room to play with
    for (int i = 0; i < u - realLower; i++)
    {
        bRep->free_area[i] = (FreeArea*)mmalloc(sizeof(FreeArea));
        bRep->free_area[i]->bit_map = bitmapnew(size, realLower+i); //0 out the bitmaps and freelists to make sure they don't have garbage in them
        bRep->free_area[i]->free_list = freelistnew(0, bRep->baseAddress, realLower, u); //calling freelistnew with 0 size will just mmalloc the freelists so they're not null
    }
    //Initialize the final level of free lists to actually have the blocks 
    bRep->free_area[u-realLower] = (FreeArea*)mmalloc(sizeof(FreeArea));
    bRep->free_area[u-realLower]->bit_map = bitmapnew(size, u-realLower); //0 out the bitmaps and freelists to make sure they don't have garbage in them
    bRep->free_area[u - realLower]->free_list = freelistnew(realSize, bRep->baseAddress, realLower, u); //Only have to give the highest order free_list items initially
    return b;
}
/*Inside of this, we will break down our free blocks into smaller free blocks to match the requested size best
We do this utilizing flags from the freelistalloc function to determine what the correct action is
-1 means that the block at a certain order was not available, check in a higher order for a free block
-2 means that we split a higher order block, and need to check a lower order freelist again to either split or allocate a block to the user
Returning an address from freelistalloc means that a block was allocated and needs to be returned to the user

*/
//Pool: pool to allocate from
//Size: Size to allocate from the pool
extern void *balloc(Balloc pool, unsigned int size)
{
    BallocRep* rep = (BallocRep*) pool;
    if(size > rep->poolSize)
        ERROR("Not enough memory to allocate");
    if(size < e2size(rep->lowerE))
        size = e2size(rep->lowerE);
    int i = 0;
    while(rep->free_area[i] != NULL && rep->free_area[i]->free_list != NULL){
        //Find the first set of free blocks to check against the size passed in by user
        //If the block size is lower than the requested size, continue in the while loop
        //If the block size is higher than the requested size, call freelistalloc which will start splitting from index i down to the proper size
        if(freelistsizeofblocks(rep->free_area[i]->free_list) >= size){
            break;
        }
        i++;
    }
    //Went all the way up the free_area array and didn't find any free blocks that would fit the requested size
    if(rep->free_area[i] == NULL)
        ERROR("No blocks are available for the requested size");
    //Finally found some blocks, let's start allocating
    void* ret = freelistalloc(rep->free_area[i]->free_list, rep->baseAddress, size, 0);
    while((long)ret == -2){ //special flag to say that we need to split
        freelistsplit(rep->free_area[i]->free_list, rep->free_area[i-1]->free_list);
        i--;
        ret = freelistalloc(rep->free_area[i]->free_list, rep->baseAddress, size, i == 0);
    }
    return ret;
}

extern void bfree(Balloc pool, void *mem)
{
    /*
    !!!! Pseudocode for partial credit !!!!
    Start looping through the bitmaps in the pool{
    test the bit 
        if(bit == 1){
            break because we found the allocation of the block
        }
    }
    freelistfree(freelist[levelFoundByLoopingThroughBitmaps], pool->base, mem, levelFoundByLooping, pool->lowerE);
        inside freelistfree, it would check if the buddy is in that level's freelist.
            if it is in that free list, return a flag to this function to merge the blocks and check if the buddy is in the next level of freelists

    Once all the blocks are merged, and the free blocks are removed from the proper freelists, we can consider that memory successuly freed

    */
    printf("BFree is not yet implemented\n");
}

extern unsigned int bsize(Balloc pool, void *mem)
{
    printf("Bsize is not yet implemented\n");
    return 0;
    //Search through the lowest to highest free_area arrays and see if an address has been allocated in that level's bitmap
    //  If it has been allocated (buddytst(mem) == 1), return that size, otherwise look in a higher order bitmap
    //  If the memory wasn't allocated (looped through all bitmaps and it wasn't allocated), return 0;
}

extern void bprint(Balloc pool)
{
    BallocRep *bRep = (BallocRep *)pool;
    printf("Size request of this pool: %d\n", bRep->poolSize);
    printf("Base address of this pool: %p\n", bRep->baseAddress);
    printf("Smallest block size: %d bytes\n", e2size(bRep->lowerE));
    printf("Largest block size: %d bytes\n", e2size(bRep->upperE));
    int i = 0;
    ////////////////////
    //FreeList Section//
    ////////////////////
    //////////////////
    printf("\nFreeList Details: \n");
    // printf("Amount of orders in FreeList: %d\n", u-l); //Maybe add this back in
    while(bRep->free_area[i] != NULL){
        freelistprint(bRep->free_area[i]->free_list, i); //Print out all the free list stuff at this orderLevel
        i++;
    }

    printf("\nBitmap Details: \n");

//////////////
////////////// This has been commented out because it's just a bunch of 0's since the bitmap wasn't implemented. Less clutter for the test suite
//////////////

    // for(int j = 0; j < i; j++){
    //     bitmapprint(bRep->free_area[j]->bit_map, bRep->poolSize, j);
    // }

    //////////////////
    //Bitmap Section//
}