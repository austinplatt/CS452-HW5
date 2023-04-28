#include <sys/mman.h>
#include <stdio.h>
#include "utils.h"
#include "error.h"
#include "freelist.h"
typedef struct FreeListNode{
    void* freeAddress; //List of free blocks at this level
    int e; //Order of magnitude of the level, definitely necessary to maintain sizes
    struct FreeListNode* next; //Link of nodes
} *FreeListNode;

typedef struct {
    FreeListNode first;
} FreeListRep;



//Merging is just check if address is on freelist, if so, remove both of them from that freelist and then add the item to the higher order freelist and run merge again on the newly added buddy

extern FreeList freelistnew(unsigned int size, void* base, int l, int u){
    FreeList fl = (FreeList)mmalloc(sizeof(FreeListRep));
    FreeListRep* flr = (FreeListRep*)fl;
    flr->first = NULL; //This ensures that our print statement works as intended
    if(size == 0)
        return fl;
    for(int i = 0; i < size/e2size(u); i++){
        FreeListNode newBlock = (FreeListNode)mmalloc(sizeof(struct FreeListNode));
        newBlock->e = u;
        newBlock->freeAddress = base + i*e2size(u);
        newBlock->next = flr->first;
        flr->first = newBlock;
    }
    return fl;
}
//Free area array
/*
typedef struct {
    FreeList fl
    BitMap
} free_area

free_area[u-l];

fl->first
*/

//Inside of this, we will break down our free blocks into smaller free blocks to match the requested size best
//We do this utilizing flags from the freelistalloc function to determine what the correct action is
//returning (void*)-1 means that the block at a certain order was not available, check in a higher order for a free block
//returning (void*)-2 means that we split a higher order block, and need to check a lower order freelist again to either split or allocate a block to the user
//Returning an address from freelistalloc means that a block was allocated and needs to be returned to the user
extern void *freelistalloc(FreeList f, void *base, unsigned int size, int bypassSizeCheck){
    FreeListRep* flr = (FreeListRep*)f;
    if(flr->first == NULL){
        fprintf(stderr, "No blocks in this level");
        ERROR("Issues");
    }
    if(e2size(flr->first->e) < size){ //Should never hit this, but this is for debugging purposes
        ERROR("Blocks are wrong size, check logic because this might indicate that we're splitting too much");
    }

    //Too much space at this level, need to split. Send the flag back to balloc for consumption
    if(e2size(flr->first->e) >= 2*size && !bypassSizeCheck){ //If we're at the bottom of the free_area array, we need to allocate a block regardless of if it's way too big
        return (void*)-2;
    }
    //GOOD TO GO. Size is right, order is right. Have blocks in the order. Remove the first block from the free list and give the memory address back to the user;
    void* retAddress = flr->first->freeAddress;
    FreeListNode nodeToFree = flr->first;
    flr->first = nodeToFree->next;
    munmap(nodeToFree, sizeof(struct FreeListNode));
    nodeToFree = 0; //0 out pointer after freeing
    return retAddress;

}
//Freelist F has some stuff on it
//We're at the highest order for elements. Only 1 block exists at this order
//We need to split that block in half, store both buddies in the next lower-order freelist
//Then we call return freelistalloc(f->lower, base, e, l);

// void* split(){
// }


extern void freelistfree(FreeList f, void *base, void *mem, int e, int l){
    printf("FreeListFree is not yet implemented");
}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){
    printf("FreeListSize is not yet implemented");
    return 0;
}


//Splits a block from inside the listToRemove and adds 2 new blocks inside listToAdd
extern int freelistsplit(FreeList listToRemove, FreeList listToAdd){
    FreeListRep* flToRemove = (FreeListRep*)listToRemove;
    //Remove the node from the list
    FreeListNode removedNode = flToRemove->first;
    flToRemove->first = removedNode->next;
    FreeListRep* flToAdd = (FreeListRep*)listToAdd;

    //Create the 2 new blocks
    FreeListNode newBlock1 = (FreeListNode)mmalloc(sizeof(struct FreeListNode));
    FreeListNode newBlock2 = (FreeListNode)mmalloc(sizeof(struct FreeListNode));
    if(newBlock1 == MAP_FAILED || newBlock2 == MAP_FAILED){
        ERROR("Unable to mmalloc the new blocks");
        return -1;
    }
    //Set the properties on the new blocks
    newBlock1->freeAddress = removedNode->freeAddress;
    newBlock1->e = removedNode->e-1; //The way the free_area structure has been created, this will never result in an exponent less than 3. Which prevents issues of minimum size being too low
    newBlock2->freeAddress = removedNode->freeAddress+e2size(newBlock1->e); //Set the address start to the original free address + an offset of the size of the block
    newBlock2->e = removedNode->e-1;
    //Add the blocks to the freeList
    newBlock1->next = newBlock2;
    newBlock2->next = flToAdd->first;
    flToAdd->first=newBlock1;

    //Finally remove the removed node from memory
    if(munmap(removedNode, sizeof(struct FreeListNode)) == -1){
        ERROR("Unable to free split node");
        return -1;
    }
    removedNode = NULL; //Null out the pointer after freeing
    return 0;

}

//Returns the size of blocks at a certain level
extern int freelistsizeofblocks(FreeList f){
    FreeListRep* flr = (FreeListRep*)f;
    if(flr == NULL){
        return 0;
    }
    FreeListNode node = flr->first;
    if(node == NULL)
        return 0; //Designated flag to say that there isn't anything in this level
    return e2size(node->e); //Return size for comparison against requested size in balloc
}

//extern void freelistprint(FreeList f, unsigned int size, int l, int u){
//Print out the list at a certain level
extern void freelistprint(FreeList f, int orderLevel){
    FreeListRep* flr = (FreeListRep*)f;
    if(flr == NULL || flr->first == NULL){
        printf("Amount of free blocks in Order %d: %d\n", orderLevel, 0);
        return;
    }
    FreeListNode node = flr->first;
    int count = 0;
    //Loop through all the nodes in this level of freelist and count them
    while(node != NULL){
        count++;
        node = node->next;
    }
    //Print amount of items at a certain order level
    printf("Amount of free blocks in Order %d: %d\n", orderLevel, count);
    printf("Size of blocks at Order %d: %d bytes\n", orderLevel, e2size(flr->first->e));
}