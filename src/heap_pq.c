/******************************************************************************
* Project: Heap pq                                                            *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Doron                                                            *
*                                                                             *
******************************************************************************/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#include "heap.h"
#include "queue.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}

#define FAIL                    1
#define SUCSSES                 0

struct queue
{
	heap_ty *heap;
};

/******************************************************************************
******************************** Queue Create ********************************/

queue_ty *QueueCreate(void)
{
	queue_ty *queue = (queue_ty *)malloc(sizeof(queue_ty));

	if(NULL == queue) return NULL; 

	queue->heap = HeapCreate();

	if(NULL == queue->heap)
	{
		FREE(queue)

		return NULL;
	}

	return queue;
}

/******************************************************************************
******************************** Queue Destroy *******************************/

void QueueDestroy(queue_ty *queue)
{
	assert(NULL != queue); 

	HeapDestroy(queue->heap);

	FREE(queue);
}

/******************************************************************************
********************************** Queue Peek ********************************/

void *QueuePeek(const queue_ty *queue)
{
	assert(NULL != queue);
	assert(!HeapIsEmpty(queue->heap));

	return HeapPeek(queue->heap); 
}

/******************************************************************************
********************************* Queue Enqueue ******************************/

int QueueEnqueue(queue_ty *queue, void *element)
{
	assert(NULL != queue);

	if(NULL == HeapPush(queue->heap, element))
	{
		return FAIL;
	}

	return SUCSSES;
}

/******************************************************************************
********************************* Queue Dequeue ******************************/

void *QueueDequeue(queue_ty *queue)
{
	void *data = NULL; 

	assert(NULL != queue);
	
	data = HeapPeek(queue->heap);

	if(NULL == HeapPop(queue->heap))
	{
		return NULL; 
	}

	return data;
}

/******************************************************************************
******************************** Queue Is Empty ******************************/

int QueueIsEmpty(const queue_ty *queue)
{
	assert(NULL != queue);

	return HeapIsEmpty(queue->heap);
}

/******************************************************************************
********************************** Queue Size ********************************/

size_t QueueSize(const queue_ty *queue)
{
	assert(NULL != queue);

	return HeapSize(queue->heap); 
}

/******************************************************************************
********************************* Queue Append *******************************/

queue_ty *QueueAppend(queue_ty *q_dest, queue_ty *q_src)
{
	assert(NULL != q_dest);
	assert(NULL != q_src);

	HeapMerge(q_dest->Heap, q_src->Heap);

	return q_dest; 
}

/*****************************************************************************/


