/******************************************************************************
* Project: Dlist                                                              *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Eli                                                              *
*                                                                             *
******************************************************************************/
#include <stdio.h>  /* puts   */
#include <stddef.h> /* NULL   */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dlist.h"

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 

#define FREE(ptr) {free(ptr); ptr = NULL;}

typedef struct dlist_node
{
	void *data;
	struct dlist_node *next;
	struct dlist_node *prev;

} dlist_node_ty;

struct dlist
{
	dlist_node_ty *head;
	dlist_node_ty *tail;
};

/*******************************************************************************
****************************** Double List Create *****************************/

/* Function Description: Create a new double linked list, including two nodes:

                         1) head node, pointing to the beginning of the list. 
                         2) tail node, pointed by the last element of the list.
                            and pointing to NULL.

                          - when the list is empty the head node is pointing to 
                            the tail node. 
 */ 

dlist_ty *DListCreate(void)
{
	dlist_iter_ty tail_node = NULL;
	dlist_ty *dlist = NULL;
	dlist_iter_ty head_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));

	if(NULL == head_node)
	{
		return NULL; 
	}

	tail_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));

	if(NULL == tail_node)
	{
		FREE(head_node)

		return NULL;
	} 

	dlist = (dlist_ty *)malloc(sizeof(dlist_ty));

	if(NULL == dlist)
	{
		FREE(tail_node)

		FREE(head_node)

		return NULL;
	}

	head_node->data = NULL; 
	head_node->prev = NULL; 
	head_node->next = tail_node; 
	
	tail_node->next = NULL;
	tail_node->prev = head_node;
	tail_node->data = NULL;

	dlist->head = head_node;
	dlist->tail = tail_node;
	
	return (dlist);
}

/*******************************************************************************
***************************** Double List Destroy *****************************/

/* Function Description: Deallocates memory used for the Double linked list. 
                         List first, then rest of supportive structures 
                         (struct, head, and tail). 
 */ 

void DListDestroy(dlist_ty *dlist)
{
	dlist_iter_ty iter = NULL;
	
	assert(NULL != dlist); 

	iter = DListBegin(dlist);

	while(iter->next != NULL) 
	{
		iter = DListRemove(iter);
	}

	FREE(dlist->tail)

	FREE(dlist->head)
	
	FREE(dlist)
}

/*******************************************************************************
****************************** Double List Insert *****************************/

/* Function Description: Insert an element to the linked list before iter.

                         1) a new node is allocated.
                         2) check if allocation fail. if fail return tail.
                         2) the new node data type is pointing to element.
                         4) the new node next type is pointing to iter.
                         5) the new node prev type is pointing to iter->prev.
                         6) new node prev, next type is pointing to new node.
                         7) iter prev type is pointing to new node.
                         
 */

dlist_iter_ty DListInsert(dlist_iter_ty iter, void *element)
{
	dlist_iter_ty new_node = NULL;

	assert(NULL != iter);

	new_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));

	if(NULL == new_node)
	{
		puts("fail to malloc dlist");

		while(NULL != iter->next)
		{
			iter = DListNext(iter); 
		}

		return iter; 
	}

	new_node->data = element;
	new_node->next = iter;
	new_node->prev = iter->prev;

	new_node->prev->next = new_node;

	iter->prev = new_node;

	return (new_node); 
}

/*******************************************************************************
**************************** Double List Push Front ***************************/

dlist_iter_ty DListPushFront(dlist_ty *dlist, void *element)
{
	dlist_iter_ty new_node = NULL;
	dlist_iter_ty begin_node = NULL;
	
	assert(NULL != dlist);

	new_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));

	if(NULL == new_node)
	{
		puts("fail to malloc dlist");

		return dlist->tail; 
	}
	
	begin_node = DListBegin(dlist);
	
	new_node->data = element;
	new_node->next = begin_node;
	new_node->prev = begin_node->prev;

	new_node->prev->next = new_node;

	begin_node->prev = new_node;

	return (new_node);  
}

/*******************************************************************************
**************************** Double List Push Back ****************************/

dlist_iter_ty DListPushBack(dlist_ty *dlist, void *element)
{

	dlist_iter_ty new_node = NULL;
	dlist_iter_ty tail_node = NULL;
	
	assert(NULL != dlist);

	new_node = (dlist_iter_ty)malloc(sizeof(dlist_node_ty));

	if(NULL == new_node)
	{
		puts("fail to malloc dlist");

		return dlist->tail; 
	}
	tail_node = DListEnd(dlist);

	new_node->data = element;
	new_node->next = tail_node;
	new_node->prev = tail_node->prev;

	new_node->prev->next = new_node;

	tail_node->prev = new_node;

	return (new_node); 
}
/*******************************************************************************
***************************** Double List Remove ******************************/
 
 /* Function Description: remove the element from the list.
                          
                          1) the next node, prev type is pointing to prev of 
                             node to remove. 
                          2) the prev node, next type is pointing to next of 
                             node to remove. 
                          3) the node is freed.
                          4) return the next node iter.  
*/

dlist_iter_ty DListRemove(dlist_iter_ty node_to_remove)
{
	dlist_iter_ty next_node = NULL;

	assert(NULL != node_to_remove);
	assert(NULL != node_to_remove->prev);
	assert(NULL != node_to_remove->next); 

	next_node = node_to_remove->next;
	next_node->prev = node_to_remove->prev; 

	node_to_remove->prev->next = next_node; 

	FREE(node_to_remove)

	return(next_node);
}

/*******************************************************************************
**************************** Double List Pop Front ****************************/

void *DListPopFront(dlist_ty *dlist)
{
	void *data = NULL;

	assert(NULL != dlist);
	assert(ILRD_TRUE != DListIsEmpty(dlist));

	data = DListGetData(DListBegin(dlist));

	DListRemove(DListBegin(dlist));

	return (data);
}

/*******************************************************************************
**************************** Double List Pop Back *****************************/

void *DListPopBack(dlist_ty *dlist)
{
	void *data = NULL;

	assert(NULL != dlist);
	assert(ILRD_TRUE != DListIsEmpty(dlist));

	data = DListGetData(DListEnd(dlist)->prev);

	DListRemove(DListEnd(dlist)->prev);

	return (data);
}

/*******************************************************************************
**************************** Double List Is Empty *****************************/

int DListIsEmpty(const dlist_ty *dlist)
{
	assert(NULL != dlist);

	return(DListBegin(dlist) == DListEnd(dlist));
}

/*******************************************************************************
****************************** Double List Size *******************************/

size_t DListSize(const dlist_ty *dlist)
{
	dlist_iter_ty next_node = NULL;
	size_t counter = 0;

	assert(NULL != dlist);

	next_node = DListBegin(dlist);

	while(next_node != DListEnd(dlist))
	{
		next_node = DListNext(next_node);
		++counter;
	}

	return (counter);
}

/*******************************************************************************
****************************** Double List Find *******************************/

/* Function Description:  find iter to the first element which obeys to 
                          specific criteria according to "match_func", in the 
                          range (from, to). 
*/
dlist_iter_ty DListFind(dlist_iter_ty from, dlist_iter_ty to, 
					                   is_match_func_ty match_func, void *param)
{
	dlist_iter_ty match_iter = from;

	assert(from);
	assert(to);
	assert(match_func);

	while(ILRD_TRUE != match_func(DListGetData(match_iter),param) && match_iter != to)
	{
		match_iter = DListNext(match_iter);
	}

	return (match_iter); 
}


dlist_iter_ty DListFindInALL(dlist_ty *dlist, is_match_func_ty match_func,
	                                                                 void *param)
{
	dlist_iter_ty from = NULL;
	dlist_iter_ty to   = NULL;

	assert(dlist);
	assert(match_func);

	from = DListBegin(dlist);
	to = DListEnd(dlist);

	return (DListFind(from, to, match_func, param)); 
}


/*******************************************************************************
**************************** Double List Multi Find ***************************/

int DListMultiFind(dlist_iter_ty from, dlist_iter_ty to, 
	            is_match_func_ty match_func, void *param, dlist_ty *output_dlist)
{
	dlist_iter_ty output_dlist_iter = NULL;
	dlist_iter_ty match_iter = from;
	dlist_iter_ty status_iter = NULL;

	assert(from);
	assert(to);
	assert(match_func);
	assert(output_dlist);

	output_dlist_iter = DListBegin(output_dlist);

	while(match_iter != to)
	{
		if(ILRD_TRUE == match_func(DListGetData(match_iter),param))
		{
			status_iter = DListInsert(output_dlist_iter, DListGetData(match_iter));

			if(DListEnd(output_dlist) == status_iter)
			{
				return FAILURE;
			}
		}		

		match_iter = DListNext(match_iter);
	}

	return (SUCSSES); 
}

/*******************************************************************************
**************************** Double List For Each *****************************/

/* Function Description: make action on every node in the linked list until 
                         failure, in the range (from, to). 
*/
int DListForEach(dlist_iter_ty from, dlist_iter_ty to, 
	                                    operation_ty action_func, void *param)

{
	int status = SUCSSES;

	assert(to);
	assert(from);
	assert(action_func);
	
	while(from != to && status == SUCSSES)
	{
		status = action_func(DListGetData(from),param);
		from = DListNext(from);
	}

	return (status);
}


int DListForEachInALL(dlist_ty * dlist, operation_ty action_func, void *param)
{
	dlist_iter_ty from = NULL;
	dlist_iter_ty to   = NULL;

	assert(dlist);
	assert(action_func);

	from = DListBegin(dlist);
	to = DListEnd(dlist);

	return (DListForEach(from, to, action_func, param)); 
}
/*******************************************************************************
**************************** Double List Splice *******************************/

/* Function Description: remove the element from the list.
                          
                          1) conecting from->prev with to.
                          2) conecting to->prev with dest->next.
                          3) conecting from with dest.

                          __________________ before ____________________


         				         |___|___|_*_|_*_|_*_|___|___|___|___|
    
						                  ^   ^       ^   ^
						          from_prev from to->prev to

					           __________________ step 1 _____________________
   

    				               |___|___|  ->   <-  |___|___|___|___|
  
						                  ^               ^
						           from->prev             to 


						                    |_*_|_*_|_*_|
 
        						                ^       ^
						                     from     to->prev


						        __________________ step 2 + 3 __________________
             

                             |___| ->  <- |_*_|_*_|_*_| -> <- |___|___|

                               ^            ^       ^           ^          
                              dest         from     to->prev   dest->next

                          ________________________________________________
*/

dlist_iter_ty DListSplice(dlist_iter_ty from, dlist_iter_ty to, dlist_iter_ty dest)
{

	dlist_iter_ty dest_next = NULL;
	dlist_iter_ty to_prev = NULL;
	dlist_iter_ty from_prev = NULL;
	
	assert(from);
	assert(to);
	assert(dest);
	
	dest_next = dest->next;
	to_prev = to->prev;
	from_prev = from->prev;

	/* step 1 - conecting from->prev with to */
	from_prev->next = to;
	to->prev = from_prev;

	/* step 2 - conecting to->prev with dest->next */
	to_prev->next = dest_next;
	dest_next->prev = to_prev;
	
	/* step 3 - conecting from with dest */
	from->prev = dest;
	dest->next = from;

	return (from);	
}

/***************************** iterator functions *****************************/


/*******************************************************************************
**************************** Double List Begin ********************************/

dlist_iter_ty DListBegin(const dlist_ty *dlist)
{
	assert(NULL != dlist);

	return (dlist->head->next); 
}

/*******************************************************************************
****************************** Double List End ********************************/

dlist_iter_ty DListEnd(const dlist_ty *dlist)
{
	assert(NULL != dlist);

	return (dlist->tail);
}

/*******************************************************************************
***************************** Double List Next ********************************/

dlist_iter_ty DListNext(const dlist_iter_ty iter)
{
	assert(iter);

	if(iter->next == NULL)
	{
		return iter;
	}

	return (iter->next);
}

/*******************************************************************************
***************************** Double List Prev ********************************/

dlist_iter_ty DListPrev(const dlist_iter_ty iter)
{
	assert(NULL != iter);
	
	if(iter->prev == NULL)
	{
		return iter;
	}

	return(iter->prev);
}

/*******************************************************************************
**************************** Double List Get Data *****************************/

void *DListGetData(dlist_iter_ty iter)
{
	assert(NULL != iter);
	
	return(iter->data);
}

/*******************************************************************************
**************************** Double List Set Data *****************************/

void DListSetData(dlist_iter_ty iter, void *data)
{
	assert(NULL != iter);
	
	iter->data = data; 
}

/*******************************************************************************
**************************** Double List Is Equal *****************************/

int DListIsEqual(const dlist_iter_ty iter1, const dlist_iter_ty iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2); 
}


/******************************************************************************/
