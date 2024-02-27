/******************************************************************************
* Project: variable size allocator                                            *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Bnaya                                                            *
*                                                                             *
******************************************************************************/
#include <assert.h>  /* assert */
#include <stdlib.h>  /* labs   */

#include "vsa.h"

#define WORD_SIZE               sizeof(void *)
#define HEADER_SIZE             sizeof(vsa_ty)

/******************** MACRO FUNCTION (for small calculation) ******************/ 

#define ALLINED_SIZE_UP(block_size) \
((((block_size) + (WORD_SIZE) - 1) / (WORD_SIZE)) *(WORD_SIZE));

#define ALLINED_SIZE_DOWN(block_size) \
(((block_size) / (WORD_SIZE)) * (WORD_SIZE));

#define MARK_AS_FREE(size)      (-1) * (size) 

#define MARK_AS_OCCUPIED(size)  (-1) * (size)

#define MAX2(a, b) (((a)>(b)) ? (a) : (b))


/******************************************************************************/

struct variable_size_allocator
{
    long block_size;
    /*debug_only(long id)*/
};

/******************************* Helper function ******************************/

static vsa_ty *FindNextNode(vsa_ty *node);
static void *AllocateFreeBlock(vsa_ty *node, long request_block_size);
static void DefragmentTwoFreeBlock(vsa_ty *node, vsa_ty *next_node);
static void DefragmentSequenceFreeBlock(vsa_ty *node, vsa_ty *next_node);


/*******************************************************************************
******************************** VSA Initialize *******************************/

/*  Function Description: Initialize the memory from user to be fvariable size 
                          allocator:
                         1) Allined the vsa size. 
                         2) Allocate space for the first maintenance struct,
                            at the begining of the user memory.
                            the struct contains the new current size of the VSA.
                         3) Allocate space for the tail header, at the end of 
                            the memory.
 
______________________ - memory pool after initilize - _________________________



     |_vsa-size-_|___________-vsa-block-after-allined-_____________|_ 0 _|
    
     ^           ^                                                 ^     ^
    head        first block                                       tail   end


________________________________________________________________________________
*/

vsa_ty *VSAInit(void *memory_vsa, size_t vsa_size)
{
    vsa_ty *head = NULL;
    vsa_ty *tail = NULL; 

    vsa_size = ALLINED_SIZE_DOWN(vsa_size)

    assert(NULL != memory_vsa);

    if(vsa_size + 1 <=  (2 * HEADER_SIZE + WORD_SIZE))
    {
        return (NULL);
    }

    /* head points to the begining of the memory */
    head = (vsa_ty *)memory_vsa;
    head->block_size = (long)(vsa_size - 2 * HEADER_SIZE);

    /* tail points to NULL */
    tail = (vsa_ty *)((char *)memory_vsa + vsa_size - HEADER_SIZE);
    tail->block_size  = 0; 

    return (head);   
}

/*******************************************************************************
********************************** VSA Alloc **********************************/

void *VSAAlloc(vsa_ty *vsa, size_t block_size)
{
    vsa_ty *node = vsa;
    vsa_ty *next_node = vsa;

    long allined_block_size = (long)ALLINED_SIZE_UP(block_size)

    assert(NULL != vsa);
    assert(0 != block_size);

   
    while(0 != node->block_size)
    {
         /* case1: current block is free & with enough space */
        if(allined_block_size <= node->block_size)
        {
            return AllocateFreeBlock(node, allined_block_size);
        }

        next_node = FindNextNode(node);

        /* case2: current block is free without enough space & next node free */
        if(0 < node->block_size && 0 < next_node->block_size)
        {
            DefragmentTwoFreeBlock(node, next_node);
            next_node = node;
        }
        
        node = next_node;
    }

    /* case3: there is no free block with enough space - return NULL */
    return NULL;
}

/*******************************************************************************
*********************************** VSA Free **********************************/

void VSAFree(void *address_to_free)
{
    vsa_ty *node = NULL;

    assert(NULL != address_to_free);

    node = (vsa_ty *)((char *)address_to_free - HEADER_SIZE);

    /* Updating the header, that the block is free */
    node->block_size = MARK_AS_FREE(node->block_size);
}

/*******************************************************************************
**************************** VSA Largest Free Block ***************************/

size_t VSALargestFreeBlock(vsa_ty *vsa)
{
    vsa_ty *node = vsa;
    vsa_ty *next_node = vsa;

    long largest_free_block = 0;

    assert(NULL != vsa);

    while(0 != node->block_size)
    {
        DefragmentSequenceFreeBlock(node, next_node);

        largest_free_block = MAX2(node->block_size,largest_free_block);

        node = FindNextNode(node);
    }

    return (largest_free_block);
}

/************************* Defragment Two Free Block **************************/

static void DefragmentTwoFreeBlock(vsa_ty *node, vsa_ty *next_node)
{     
    node->block_size += next_node->block_size + HEADER_SIZE;
}

/********************** Defragment Sequence Free Block ************************/

static void DefragmentSequenceFreeBlock(vsa_ty *node, vsa_ty *next_node)
{
    next_node = FindNextNode(node);
        
    while(0 < node->block_size && 0 < next_node->block_size)
    {
        node->block_size += next_node->block_size + HEADER_SIZE;

        next_node = FindNextNode(node);
    }
}

/******************************* Find Next Node *******************************/
 
static vsa_ty *FindNextNode(vsa_ty *node)
{
    return ((vsa_ty *)((char *)node + labs(node->block_size) 
                                                        + HEADER_SIZE));
}


/**************************** Allocate Free Block *****************************/

static void *AllocateFreeBlock(vsa_ty *node, long request_block_size)
{
    vsa_ty *next_node = node; 
    long size_of_current_block = node->block_size;

    /* Avoiding the case of create a block size 0 */
    if(size_of_current_block == request_block_size + (long)HEADER_SIZE)
    {
        request_block_size += (long)HEADER_SIZE;
    }

    /* Updating the hader with the number of bytes to be occupied */
    node->block_size = MARK_AS_OCCUPIED(request_block_size);

    /* creating a new hader with the number of bytes left free */
    if(size_of_current_block > request_block_size)
    {
        next_node = FindNextNode(node);
        next_node->block_size = size_of_current_block 
                                     - request_block_size - HEADER_SIZE;
    }

    return ((void *)((char *)(node) + HEADER_SIZE)); 
}
