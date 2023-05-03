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

/**
 * Initializes a new freeList
 */ 
extern FreeList freelistnew(unsigned int size, void* base, int l, int u){
    int numLists = u-l;
    
    memset(&list, 0, sizeof(FreeListSpace) *numLists);

    //Allocates each level a bitmap
    for(int i =0; i <= numLists; i++){
        int levelSize = size2e(i + l);
        int levelE = e2size(levelSize);
        list[i].map = bitmapnew(levelE, levelSize);
    }

    int totalE = size2e(size);
    //Difference between the expected and actual block size
    int deltaE = totalE - u;

    if(!deltaE){
        list[numLists].head = base;
    }
    //If u is less than size, we must immediately divide the blocks.
    else{
       split(list, base, base, 0, deltaE, u, l );
       updateHead(list, base, numLists);
    }
    return (FreeList)list;
}

/**
 * Splits the block into the specified level
 */ 
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

/**
 * Splits the block into the suggested init level
 */ 
static void split(FreeList f, void *base, void* mem, int i, int deltaE, int u, int l){
    void* buddy = buddyinv(base, mem, u + deltaE -i -1);
    while(deltaE/2 > i){
        split(f, base, mem, i+1, deltaE, u, l);
        split(f, base, buddy, i+1, deltaE, u, l);
        i++;
    }
    updateHead(f, buddy, u-l);
}

/**
 * Swaps the heads with a new piece of Memory
 */ 
static void updateHead(FreeList f, void* mem, int index){
    FreeListSpace *rep = (FreeListSpace *)f;
    void* currentHead = rep[index].head;
    FreeBlock* memRep = (FreeBlock*)mem;

    memRep->next = currentHead;
    rep[index].head = memRep;
}

/**
 * Function that allocates memory from freelist
 */ 
extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){
    FreeListSpace *rep = (FreeListSpace *)f;
    
    void *allocatedAddress = NULL;

    int startLevel = e - l;
    int endLevel = u - l;
    int index = startLevel;

    FreeListSpace currSpace = rep[startLevel];
    
    while (currSpace.head == NULL && index <= endLevel)
    {
        currSpace = rep[index];
        index++;
    }
    int foundIndex = startLevel == index ? startLevel : index - 1;
    allocatedAddress = currSpace.head;

    if (foundIndex > startLevel && rep[foundIndex].head != NULL)
    {
        splitToLevel(f, base, allocatedAddress, startLevel, foundIndex, l);
    }
    bitmapset(rep[startLevel].map, base, allocatedAddress, e);
    FreeBlock *initialBlock = rep[foundIndex].head;
    rep[foundIndex].head = initialBlock->next;
    return allocatedAddress;
}

/**
 * Prints a Node
 */ 
extern void printNode(void* node){
  FreeBlock* next = (FreeBlock*)node;
  while(next != NULL){
      printf("%p -> %p\n", next, next->next);
      next = next->next;
  }
}

/**
 * Functions frees a block of memory and stores it in the freelist
 */ 
extern void freelistfree(FreeList f, void *base, void *mem, int e, int l){
    FreeListSpace* level = (FreeListSpace*) &list[e-l];
    
    void* buddy = buddyinv(base, mem, e);
    FreeBlock* nxtNode = (FreeBlock*)level->head;
    FreeBlock* prevNode = NULL;

    while(nxtNode != NULL){
        if(nxtNode == buddy){
            break;
        }
        prevNode = nxtNode;
        nxtNode = nxtNode->next;
    }
    if(nxtNode != NULL){
        bitmapclr(level->map, base, mem, e);
        if(nxtNode == level->head){
            level->head = nxtNode->next;
        }
        else{
            prevNode->next = nxtNode->next;
        }
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

/**
 * Determines the side of the block
 */ 
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

/**
 * Function that prints out the freelist
 */ 
extern void freelistprint(FreeList f, unsigned int size, int l, int u){
   FreeListSpace *rep = (FreeListSpace *)f;  
   int numLists = u-l;
   for(int i=0; i<=numLists; i++){
       printf("Level: %d\n", i);
       printNode(rep[i].head);
       bitmapprint(rep[i].map, size, size2e(i+l));
   }
}