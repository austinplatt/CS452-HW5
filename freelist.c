#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

#include "freelist.h"
#include "utils.h"
#define MAX_LEVELS 20
#include "bitmap.h"

// Struct to hold freelists
typedef struct{
    void* head;
    BitMap map;
}FreeListSpace;

// Struct to access first 8 bits of a free block
typedef struct{
    void* next;
}FreeBlock;

static FreeListSpace list[20];

// <summary>Initializes a new freelist</summary>
// <param name = "size">Size of the initial map</param>
// <param name = "base">Base address mmap</param>
// <param name = "l">Low end</param>
// <param name = "u">High end</param>
// <returns>A void* to a allocated freelist</returns>
extern FreeList freelistnew(unsigned int size, void* base, int l, int u){
    int numLists = u-l;
    
    memset(&list, 0, sizeof(FreeListSpace) *numLists);

    // Allocates each level a bitmap
    for(int i =0; i <= numLists; i++){
        int levelSize = size2e(i + l);
        int levelE = e2size(levelSize);
        list[i].map = bitmapnew(levelE, levelSize);
    }

    int totalE = size2e(size);
    // Difference between u and acutal size of block
    int deltaE = totalE - u;

    // If we have same u as size
    if(!deltaE){
        list[numLists].head = base;
    }
    // If we have less u than size we need to split up blocks initially
    else{
       split(list, base, base, 0, deltaE, u, l );
       updateHead(list, base, numLists);
    }
    return (FreeList)list;
}

// <summary>Splits a block to the level specified</summary>
// <param name = "base">base address returned by mmap</param>
// <param name = "mem">Block to split</param>
// <param name = "sLevel">Level that we started searching from</param>
// <param name = "currLevel">Iteration level we are on</param>
// <param name = "offset">array offset from e</param>
static void splitToLevel(FreeList f, void *base, void* mem, int sLevel, int currLevel, int offset){
    FreeListSpace *rep = (FreeListSpace *)f;
    FreeBlock* buddy = NULL;
    while(currLevel > sLevel){
        void* buddy = buddyinv(base, mem, (offset + currLevel - 1));
        if(!bitmaptst(rep[currLevel].map, base, buddy, (offset+ currLevel - 1))){
            updateHead(f, buddy, currLevel-1);
        }
        currLevel--;
    }
}

// <summary>Splits a block to the level on init</summary>
// <param name = "base">base address returned by mmap</param>
// <param name = "mem">Block to split</param>
// <param name = "i">Iteration level we are on</param>
// <param name = "u">Upper bound</param>
// <param name = "l">Lower bound</param>
static void split(FreeList f, void *base, void* mem, int i, int deltaE, int u, int l){
    void* buddy = buddyinv(base, mem, u + deltaE -i -1);
    while(deltaE/2 > i){
        split(f, base, mem, i+1, deltaE, u, l);
        split(f, base, buddy, i+1, deltaE, u, l);
        i++;
    }
    updateHead(f, buddy, u-l);
}

// <summary>Swaps head with a new chunk of memory</summary>
// <param name = "f">Free list</param>
// <param name = "mem">Memory to be new head</param>
// <param name = "index">Index of array to swap</param>
static void updateHead(FreeList f, void* mem, int index){
    FreeListSpace *rep = (FreeListSpace *)f;
    void* currentHead = rep[index].head;
    FreeBlock* memRep = (FreeBlock*)mem;

    memRep->next = currentHead;
    rep[index].head = memRep;
}

// <summary>Allocates block from freelist</summary>
// <param name = "base">base address returned by mmap</param>
// <param name = "e">Order</param>
// <param name = "l">lower level</param>
// <param name = "u">upper level</param>
// <returns>Address of the allocated block</returns>
extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){
    FreeListSpace *rep = (FreeListSpace *)f;
    
    void *allocatedAddress = NULL;

    // Calculates offsets
    int startLevel = e - l;
    int endLevel = u - l;
    int index = startLevel;

    FreeListSpace currSpace = rep[startLevel];
    
    // We need u so we know if we don't have enough memory to return to user
    while (currSpace.head == NULL && index <= endLevel)
    {
        currSpace = rep[index];
        index++;
    }
    // Figures out what index we found the block at
    int foundIndex = startLevel == index ? startLevel : index - 1;
    allocatedAddress = currSpace.head;

    if (foundIndex > startLevel && rep[foundIndex].head != NULL)
    {
        // Splits block if needed
        splitToLevel(f, base, allocatedAddress, startLevel, foundIndex, l);
    }
    // Sets bitmap so we can figure out size
    bitmapset(rep[startLevel].map, base, allocatedAddress, e);
    FreeBlock *initialBlock = rep[foundIndex].head;
    rep[foundIndex].head = initialBlock->next;
    return allocatedAddress;
}

// <summary>Prints a node</summary>
// <param name = "node">Address of block/node to print</param>
extern void printNode(void* node){
  FreeBlock* next = (FreeBlock*)node;
  while(next != NULL){
      printf("%p -> %p\n", next, next->next);
      next = next->next;
  }
}

// <summary>Frees a block of memory and returns it to the freelist</summary>
// <param name = "base">base address returned by mmap</param>
// <param name = "mem">Block to split</param>
// <param name = "e">Order</param>
// <param name = "l">Lower level</param>
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l){
    FreeListSpace* level = (FreeListSpace*) &list[e-l];
    
    // Gets buddy
    void* buddy = buddyinv(base, mem, e);
    FreeBlock* nxtNode = (FreeBlock*)level->head;
    FreeBlock* prevNode = NULL;

    // Checks next nodes
    while(nxtNode != NULL){
        if(nxtNode == buddy){
            break;
        }
        prevNode = nxtNode;
        nxtNode = nxtNode->next;
    }
    if(nxtNode != NULL){
        // Resets bit map for mem address
        bitmapclr(level->map, base, mem, e);
        if(nxtNode == level->head){
            level->head = nxtNode->next;
        }
        else{
            prevNode->next = nxtNode->next;
        }
        // Gets the base buddy
        void* buddyBase = buddyclr(base, mem, e);
        freelistfree(f, base, buddyBase, e+1, l);
    }
    else{
        void* temp = level->head;
        FreeBlock* memN = (FreeBlock*)mem;
        memN->next = temp;
        level->head = mem;
    }
}

// <summary>Figures out side of an allocated block</summary>
// <param name = "base">base address returned by mmap</param>
// <param name = "mem">Block to look up</param>
// <param name = "l">lower level</param>
// <param name = "u">upper level</param>
// <returns>size of block</returns>
extern int freelistsize(FreeList f, void *base, void *mem, int l, int u)
{
    FreeListSpace *rep = (FreeListSpace *)f;  
    int size = 0;
    int numLists = u-l;
    int counter = 0;

    while(counter <= u){
        int e = counter + l;
        if(bitmaptst(rep[counter].map, base, mem, e)){
            size = e2size(e);
            break;
        }
        counter++;
    }
    return size;
}

// Prints free list
extern void freelistprint(FreeList f, unsigned int size, int l, int u){
   FreeListSpace *rep = (FreeListSpace *)f;  
   int numLists = u-l;
   for(int i=0; i<=numLists; i++){
       printf("Level: %d\n", i);
       printNode(rep[i].head);
       bitmapprint(rep[i].map, size, size2e(i+l));
   }
}