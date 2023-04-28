#ifndef FREELIST_H
#define FREELIST_H

typedef void *FreeList;

extern FreeList freelistnew(unsigned int size, void* base, int l, int u);

//Use the free list to maintain free blocks while removing an element from the list and returning the address to the user
extern void *freelistalloc(FreeList f, void *base, unsigned int size, int bypassSizeCheck);
//Free an element in a freelist at a certain address
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l);
//Return size of memory in a certain free list if it exists
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);

// extern void freelistprint(FreeList f, unsigned int size, int l, int u);
extern void freelistprint(FreeList f, int orderLevel);

//Get size of the free blocks at a certain level
extern int freelistsizeofblocks(FreeList f);

//Function to split one block into two blocks
extern int freelistsplit(FreeList listToRemove, FreeList listToAdd);

#endif