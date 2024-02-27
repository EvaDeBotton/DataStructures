/******************************************************************************
* Project: Heap data structure                                                *
*                                                                             *
* Wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by Doron Ben-David                                                  *
*                                                                             *
******************************************************************************/
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "heap.h"
#include "vector.h"
#include "utils.h"

#define PARENT(index)      (((index + 1) / 2) - 1); 
#define LEFT_CHILD(index)  ((2 * (index + 1)) - 1);
#define RIGHT_CHILD(index) ((2 * (index + 1)));

/* colors for debugging */
#define RED			                    "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                         "\033[0;37m"

#define FREE(ptr) {free(ptr); ptr = NULL;} 
#define MALLOC(nelements, type) ((type *)malloc(sizeof(type) * nelements));
#define MAX(num_1, num_2) ((num_1) > (num_2) ? (num_1) : (num_2));

enum is_empty 
{
   NOT_EMPTY = 0, 
   EMPTY  = 1 
};

enum return_status 
{
   ILRD_SUCSSES = 0, 
   ILRD_FAILURE = 1 
};

enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum match_status
{
	NO_FOUND_MATCH,
	FOUND_MATCH 
};

struct heap
{
   heap_cmp_func_ty cmp_func;
   vector_ty *vector;
};

static void HeapifyUp(heap_ty *heap, size_t index);
static void HeapifyDown(heap_ty *heap, size_t index);

static void SwapData(size_t index, size_t right_child_index, heap_ty *heap);
static size_t HeapFind(heap_ty *heap, void *param, is_match_func_ty match_func);

/*******************************************************************************
********************************** Heap Create ********************************/

heap_ty *HeapCreate(heap_cmp_func_ty cmp_func, size_t capacity)
{
	heap_ty *new_heap = NULL;

	assert(NULL != cmp_func);

	new_heap = MALLOC(1, heap_ty);

	if(NULL == new_heap)
	{
		return NULL; 
	} 

	new_heap->vector = VectorCreate(capacity);

	if(NULL == new_heap->vector)
	{
		HeapDestroy(new_heap);
		
		FREE(new_heap);
		return NULL; 
	} 

	new_heap->cmp_func = cmp_func;

	return new_heap;
}

/*******************************************************************************
********************************** Heap Destroy *******************************/

void HeapDestroy(heap_ty *heap)
{
	assert(NULL != heap);
	assert(NULL != heap->vector);

	VectorDestroy(heap->vector);

	FREE(heap);
}

/*******************************************************************************
************************************ Heap Push ********************************/

int HeapPush(heap_ty *heap, void *new_data)
{
	size_t lest_index = 0; 

	assert(NULL != heap);

	/* step 1 : The new data is placed at the end of the array */
	if(FAILURE == VectorPushBack(heap->vector, new_data))
	{
		return FAILURE;
	}

	/* step 2 : The new data is heapify up to its correct position */
	lest_index = HeapSize(heap) - 1;
	HeapifyUp(heap, lest_index);
		
	return SUCSSES;
}

/*******************************************************************************
************************************ Heap Pop *********************************/

int HeapPop(heap_ty *heap)
{
	size_t lest_index = 0;

	void *lest_data = NULL;

	assert(NULL != heap);

	/* step 1 : Saving aside the data of the lest element 
	*/
	lest_index = HeapSize(heap) - 1;
	lest_data  = VectorGetVal(heap->vector, lest_index);

	/* step 2 : Remove the lest element from the vector 
	*/
	if(FAILURE == VectorPopBack(heap->vector))
	{
		return FAILURE;
	}

	/* step 3 : place the lest element data insted the first element data 
	*/
	VectorSetVal(heap->vector, 0, lest_data);

	/* step 4 : The lest element data is heapify down to its correct position 
	*/
	HeapifyDown(heap, 0);
		
	return SUCSSES;
}

/*******************************************************************************
************************************ Heap Peek ********************************/

void *HeapPeek(const heap_ty *heap)
{
	assert(NULL != heap);
	if(EMPTY == HeapIsEmpty(heap))
	{
		return NULL;
	}

	return (VectorGetVal(heap->vector, 0));
}

/*******************************************************************************
************************************ Heap Size ********************************/

size_t HeapSize(const heap_ty *heap)
{
	assert(NULL != heap);

	return (VectorSize(heap->vector));
}

/*******************************************************************************
********************************** Heap Is Empty ******************************/

int HeapIsEmpty(const heap_ty *heap)
{
	assert(NULL != heap);

	return (0 == VectorSize(heap->vector));
}

/*******************************************************************************
*********************************** Heap Remove *******************************/

void *HeapRemove(heap_ty *heap, void *param, is_match_func_ty match_func)
{
	size_t last_index = 0;
	size_t index_data_to_remove = 0;
	size_t parent_index = 0;

	void *last_data = NULL;
	void *data_to_remove = NULL;
	void *parent_data = NULL;

	assert(NULL != heap);
	assert(NULL != match_func);

	/* step 1 : Finding the index of the element to removed
	 */
	index_data_to_remove = HeapFind(heap, param, match_func);

	if(HeapSize(heap) == index_data_to_remove)
	{
		return NULL;
	}

	data_to_remove = VectorGetVal(heap->vector, index_data_to_remove);

	/* step 2 : Saving aside the data of the last element and remove it 
					from the vector.
	*/
	last_index = HeapSize(heap) - 1;

	last_data = VectorGetVal(heap->vector, last_index);

	if(FAILURE == VectorPopBack(heap->vector))
	{
		return NULL;
	}

	/* step 3 : place the last element data in the position of the element 
					to remove. 
	*/
	VectorSetVal(heap->vector, index_data_to_remove, last_data);

	/* step 4 : The last element data is heapify to its correct position 
	*/
	if(index_data_to_remove != 0)
	{
		parent_index = PARENT(index_data_to_remove);
		parent_data = VectorGetVal(heap->vector, parent_index);

		if(0 < heap->cmp_func(last_data ,parent_data))
		{
			HeapifyUp(heap, index_data_to_remove);
		}
		else
		{
			HeapifyDown(heap, index_data_to_remove);
		}
	}
	else
	{
		HeapifyDown(heap, index_data_to_remove);
	}

	return data_to_remove;
}

/*******************************************************************************
******************************* staic - Heapify Up ****************************/

static void HeapifyUp(heap_ty *heap, size_t child_index)
{
	size_t parent_index = 0;

	void *parent_data = NULL;
	void *child_data  = NULL;

	assert(NULL != heap);

	if(0 == child_index)
	{
		return;
	} 

	parent_index = PARENT(child_index);

	parent_data = VectorGetVal(heap->vector, parent_index);
	child_data  = VectorGetVal(heap->vector, child_index);

	while(0 < heap->cmp_func(child_data ,parent_data))
	{
		SwapData(child_index, parent_index, heap);

		/* stop condition - if the parent index is the root */ 
		if(parent_index == 0)
		{
			break;
		}

		child_index = parent_index;
		parent_index = PARENT(child_index);

		parent_data = VectorGetVal(heap->vector, parent_index);
		child_data  = VectorGetVal(heap->vector, child_index);
	}
}

/*******************************************************************************
****************************** static - Heapify Down **************************/

static void HeapifyDown(heap_ty *heap, size_t perant_index)
{
	size_t left_child_index  = 0;
	size_t right_child_index = 0;

	void *parent_data = NULL;
	void *left_child_data  = NULL;
	void *right_child_data = NULL;

	assert(NULL != heap);

	left_child_index  = LEFT_CHILD(perant_index);
	right_child_index = RIGHT_CHILD(perant_index);

	/* check if at least one child */
	while(right_child_index <= (HeapSize(heap)))
	{
		/* case 1 : the parent have only one child 
		*/
		if(left_child_index == (HeapSize(heap) - 1))
		{
			parent_data = VectorGetVal(heap->vector, perant_index);
			left_child_data = VectorGetVal(heap->vector, left_child_index);

			if(0 < heap->cmp_func(left_child_data, parent_data))
			{
				SwapData(perant_index, left_child_index, heap);
			}
			
			/* stop condition - end of the list 
			*/
			return;
		}

		/* case 2 : the parent have two childrens 
		*/
		parent_data = VectorGetVal(heap->vector, perant_index);
		left_child_data  = VectorGetVal(heap->vector, left_child_index);
		right_child_data = VectorGetVal(heap->vector, right_child_index);

		/* if negative right child is bigger 
		*/
		if(0 <= heap->cmp_func(left_child_data, right_child_data))
		{
			if(0 < heap->cmp_func(right_child_data, parent_data))
			{
				SwapData(perant_index, left_child_index, heap);
				perant_index = left_child_index;
			}
			else
			{
				/* stop condition - non of the children is bigger 
					than the parent 
				*/
				return;
			}
		}
		else
		{
			if(0 < heap->cmp_func(right_child_data, parent_data))
			{
				SwapData(perant_index, right_child_index, heap);
				perant_index = right_child_index;
			}
			else
			{
				/* stop condition - non of the children is bigger 
					than the parent 
				*/
				return;
			}
		}

		left_child_index  = LEFT_CHILD(perant_index);
		right_child_index = RIGHT_CHILD(perant_index);
	}
}

/*******************************************************************************
******************************* static - Swap Data ****************************/

static void SwapData(size_t index_1, size_t index_2, heap_ty *heap)
{
	void *data_1 = NULL; 
	void *data_2 = NULL; 

	assert(NULL != heap);

	data_1 = VectorGetVal(heap->vector, index_1);
	data_2 = VectorGetVal(heap->vector, index_2);

	VectorSetVal(heap->vector, index_1, data_2);
	VectorSetVal(heap->vector, index_2, data_1);
}

/*******************************************************************************
******************************* static - Heap Find ****************************/

static size_t HeapFind(heap_ty *heap, void *param, is_match_func_ty match_func)
{
	void *data = NULL; 

	size_t index = 0;

	while(index < (HeapSize(heap)))
	{
		data = VectorGetVal(heap->vector, index);

		if(FOUND_MATCH == match_func(data,param)) 
		{
			return index;
		}

		++index; 
	}

	return (HeapSize(heap));
}
