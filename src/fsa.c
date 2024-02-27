/******************************************************************************
* Project: fixed size allocator                                               *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Doron                                                            *
*                                                                             *
******************************************************************************/
#include <assert.h>  /* assert */

#include "fsa.h"

#define WORD_SIZE       sizeof(void *)
#define HEADER_SIZE     sizeof(fsa_ty)

#define ALLINED_BLOCK_SIZE(block_size)\
{(((block_size) + (WORD_SIZE) - 1) / (WORD_SIZE)) * (WORD_SIZE)}\

struct fsa_ty
{
    void *next;

}fsa_ty;


/*******************************************************************************
******************************** FSA Initialize *******************************/

/*  Function Description: Initialize the memory from user to be fixed size 
                          allocator:

                         1) allocate space for the maintenance struct, at the
                            start of the user memory. 
                            The size of the struct is same size as the size of 
                            (void*) in bytes, it contains the head of the free 
                            list. 
                         2) determine the location of the pool blocks and the 
                            free list one by one. each pointer is stored in the  
                            begining of each block.
 


______________________ - memory pool after initilize - _________________________



     |_&A_|_&B___-block-______|_&C___-block-______|_NULL___-block-_|
    
     ^    ^                   ^                   ^                ^
    head  A                   B                   C               end




________________________________________________________________________________
*/

fsa_ty *FSAInit(void *memory_fsa, size_t pool_size, size_t block_size)
{
    fsa_ty *head = NULL;
    fsa_ty *node = NULL;

    size_t num_of_blocks = (pool_size - HEADER_SIZE)/(block_size);

    block_size = ALLINED_BLOCK_SIZE(block_size)

    /* check if the address is allined */ 
    assert(0 == (size_t)memory_fsa % WORD_SIZE);

    assert(NULL != memory_fsa);
    assert(0 < num_of_blocks);

    /* head points to the first free block */
    head = (fsa_ty *)memory_fsa;
    head->next = (void *)((char *)memory_fsa + HEADER_SIZE);

    /* loop for initializing the free list, and the blocks start */
    node = head->next

    while(1 != num_of_blocks)
    {
        node->next = (void *)((char *)node + block_size);
        node = (fsa_ty *)(node->next);

        --num_of_blocks;
    }

    node->next = NULL;

    return (head);
    
}

/*******************************************************************************
********************************** FSA Alloc **********************************/

void *FSAAlloc(fsa_ty *head)
{
    fsa_ty *block_ptr = {0};

    assert(NULL != head);

    if(NULL == head->next)
    {
        return NULL;
    }

    block_ptr = (fsa_ty *)head->next;

    head->next = (void *)(block_ptr->next);

    return (block_ptr);
}

/*******************************************************************************
*********************************** FSA Free **********************************/

void FSAFree(fsa_ty *head, void *address_to_free)
{
    fsa_ty *node = {0};

    assert(NULL != head);
    assert(NULL != address_to_free);

    node = (fsa_ty *)address_to_free;

    node->next = (void *)head->next;

    head->next = address_to_free;
}

/*******************************************************************************
******************************** FSA Free Blocks ******************************/

size_t FSAFreeBlocks(const fsa_ty *head)
{
    size_t counter = 0;
    fsa_ty *node = head;

    assert(NULL != head);

    while(NULL != node->next)
    {
        node = (fsa_ty *)node->next;

        ++counter;
    }

    return counter;
}

/*******************************************************************************
********************************* Suggested Size ******************************/

size_t SuggestedSize(size_t block_size, size_t num_of_blocks)
{
    block_size = ALLINED_BLOCK_SIZE(block_size);
 
     /* total size = number of block * allined size of block  + head pointer */
    return (num_of_blocks * block_size + HEADER_SIZE);
}
