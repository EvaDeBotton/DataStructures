/******************************************************************************
* Project: Queue                                                              *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Bnaya                                                            *
*                                                                             *
******************************************************************************/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#include "slist.h"
#include "queue.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}

#define FAIL                    1
#define SUCSSES                 0

struct queue
{
	slist_ty *slist;
};

/******************************************************************************
******************************** Queue Create ********************************/

queue_ty *QueueCreate(void)
{
	queue_ty *queue = (queue_ty *)malloc(sizeof(queue_ty));

	if(NULL == queue) return NULL; 

	queue->slist = SlistCreate();

	if(NULL == queue->slist)
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

	SlistDestroy(queue->slist);

	free(queue);
}

/******************************************************************************
********************************** Queue Peek ********************************/

void *QueuePeek(const queue_ty *queue)
{
	assert(NULL != queue);
	assert(!SlistIsEmpty(queue->slist));

	return SlistGetData(SlistBegin(queue->slist)); 
}

/******************************************************************************
********************************* Queue Enqueue ******************************/

int QueueEnqueue(queue_ty *queue, void *element)
{
	assert(NULL != queue);

	if(NULL == SlistInsert(element, SlistEnd(queue->slist)))
	{
		return FAIL;
	}

	return SUCSSES;
}

/******************************************************************************
********************************* Queue Dequeue ******************************/

void QueueDequeue(queue_ty *queue)
{
	assert(NULL != queue);

	SlistRemove(SlistBegin(queue->slist));
}

/******************************************************************************
******************************** Queue Is Empty ******************************/

int QueueIsEmpty(const queue_ty *queue)
{
	assert(NULL != queue);

	return SlistIsEmpty(queue->slist);
}

/******************************************************************************
********************************** Queue Size ********************************/

size_t QueueSize(const queue_ty *queue)
{
	assert(NULL != queue);

	return SlistCount(queue->slist); 
}

/******************************************************************************
********************************* Queue Append *******************************/

queue_ty *QueueAppend(queue_ty *q_dest, queue_ty *q_src)
{
	assert(NULL != q_dest);
	assert(NULL != q_src);

	SlistMerge(q_dest->slist, q_src->slist);

	return q_dest; 
}

/*****************************************************************************/


