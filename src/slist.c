/******************************************************************************
* slist.c                                                                     *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Eli                                                              *
*                                                                             *
******************************************************************************/
#include <stdlib.h> /* malloc free */
#include <stdio.h> /* puts, printf */
#include <assert.h> /* assert */
#include "slist.h"

#define TRUE                             1
#define FALSE                            0

#define FREE(ptr) {free(ptr); ptr = NULL;}

enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
};  

struct slist
{
	slist_iter_ty head; 
	slist_iter_ty tail; 
};

struct slist_node
{
	void *data; 
	slist_iter_ty next; 
};

/******************************************************************************
******************************** Slist Destroy *******************************/

/* Function Description: Deallocates memory used for the linked list. 
                         List first, then rest of supportive structures 
                         (struct, head, and tail). 
 */ 

void SlistDestroy(slist_ty *slist)
{
	slist_iter_ty iter = NULL;
	
	assert(NULL != slist); 

	iter = SlistBegin(slist);

	while(SlistNext(iter) != NULL) 
	{
		iter = SlistRemove(iter);
	}

	FREE(slist->tail)

	FREE(slist->head)
	
	FREE(slist)
}

/******************************************************************************
******************************** Slist Insert ********************************/

/* Function Description: Insert an element to the linked list before iter.

                         1) a new node is allocated.
                         2) the data of iter is transferred to the new node.
                         3) the new data is transferred to the data of the iter
                            node.
                         4) the new node is pointing to iter.  
                         
 */
slist_iter_ty SlistInsert(void *element, slist_iter_ty iter)
{
	slist_iter_ty new_node = (slist_iter_ty )malloc(sizeof(slist_node_ty));

	assert(iter);

	if(NULL == new_node)
	{
		puts("fail to malloc slist");

		while(0 != SlistNext(iter))
		{
			iter = SlistNext(iter); 
		}
		return iter; 
	}
	
	new_node->data = element;
	new_node->next = iter;
	new_node->next = iter;
	
	
	iter->next = new_node;
	
	if (new_node->next == NULL)
	{
		address_slist = new_node->data;
		address_slist->tail = new_node;
	}

	return iter; 
}

/******************************************************************************
******************************** Slist Remove ********************************/
 
 /* Function Description: remove the element from the list.
                          
                          1) the data of the next node is transferred to the 
                             node thats goning to be removed. 
                          2) the iter pointer is reallocated to the node that
                             comes after the next node. 
                          3) the next node is freed. 
*/ 
slist_iter_ty SlistRemove(slist_iter_ty iter)
{
	slist_iter_ty next_iter = SlistNext(iter);
	slist_ty *slist_ptr = NULL;

	assert(iter);

	iter->data = next_iter->data; 
	iter->next = SlistNext(next_iter);
		
	if(NULL == SlistNext(iter))
	{
		slist_ptr = iter->data; 
		slist_ptr->tail = iter;
	}

	FREE(next_iter)

	return iter;
}
/******************************************************************************
******************************* Slist Is Empty *******************************/

/* Function Description: return if slist is empty. 
*/ 
bool_ty SlistIsEmpty(const slist_ty *slist)
{
	assert(slist);

	 return ((SlistBegin(slist) == SlistEnd(slist)) ? TRUE : FALSE);
}

/******************************************************************************
********************************* Slist Count ********************************/

/* Function Description:  Return the number of elements in the slist.
*/ 
size_t SlistCount(const slist_ty *slist)
{
	slist_iter_ty current_ptr = NULL;
	size_t counter = 0; 

	assert(NULL != slist);
 	
 	current_ptr = SlistBegin(slist); 
 	/* change back cu->next*/
	while(NULL != SlistNext(current_ptr)) 
	{
		++counter; 
		current_ptr = SlistNext(current_ptr); 
	}

	return counter; 
}

/******************************************************************************
********************************* Slist Find *********************************/

/* Function Description:  find iter to the first element which obeys to 
                          specific criteria according to "match_func", in the 
                          range [from, to). 
*/
slist_iter_ty SlistFind(slist_iter_ty from, slist_iter_ty to, 
	                                  is_match_func_ty match_func, void *param)
{
	slist_iter_ty match_iter = from;

	assert(from);
	assert(to);
	assert(match_func);

	while(TRUE != match_func(SlistGetData(match_iter),param) && match_iter != to)
	{
		match_iter = SlistNext(match_iter);
	}

	return match_iter; 
}

/******************************************************************************
******************************* Slist For Each *******************************/

/* Function Description: make action on every node in the linked list until 
                         failure, in the range [from, to). 
*/
int SlistForEach(slist_iter_ty from, slist_iter_ty to, 

	                                   action_func_ty action_func, void *param) 
{
	int status = SUCSSES;

	assert(to);
	assert(from);
	assert(action_func);
	
	while(from < to && status == SUCSSES)
	{
		status = action_func(SlistGetData(from),param);
		from = SlistNext(from);
	}

	return status;
}

/******************************************************************************
********************************* Slist Begin ********************************/

/* Function Description: return iterator to first element of linked list.
*/
slist_iter_ty SlistBegin(const slist_ty *slist)
{
	assert(slist);

	return slist->head->next; 
}

/******************************************************************************
********************************** Slist End *********************************/

/* Function Description: return iterator to dummy end element of linked list.
*/

slist_iter_ty SlistEnd(const slist_ty *slist)
{
	assert(slist);

	return slist->tail; 
}

/******************************************************************************
********************************* Slist Next *********************************/

/* Function Description: return next iterator linked list.
*/
slist_iter_ty SlistNext(slist_iter_ty iterator)
{
	assert(iterator);

	return iterator->next; 
}

/******************************************************************************
******************************* Slist Get Data *******************************/

/* Function Description: return data of given iterator.
*/ 
void *SlistGetData(slist_iter_ty iterator)
{
	assert(iterator);

	return iterator->data; 
}

/******************************************************************************
******************************* Slist Set Data *******************************/

/* Function Description: set data to given iterator.
*/
void SlistSetData(slist_iter_ty iterator, void *data)
{ 
	assert(iterator);

	iterator->data = data; 
}

/******************************************************************************
******************************* Slist Is Equal *******************************/

/* Function Description: check if both iterators are for the same element.
*/

bool_ty SlistIsEqual(slist_iter_ty iterator1, slist_iter_ty iterator2)
{
	assert(iterator1);
	assert(iterator2);
	
	return ((iterator1 == iterator2) ? TRUE : FALSE); 
}

/******************************************************************************
********************************** Slist Merge *******************************/

slist_ty *SlistMerge(slist_ty *dest,slist_ty *src)
{
	slist_iter_ty src_begin = NULL;
	slist_iter_ty src_tail = NULL;
	slist_iter_ty dest_tail = NULL;

	assert(dest);
	assert(src);

	src_begin = SlistBegin(src);
	src_tail = SlistEnd(src);
	dest_tail = SlistEnd(dest);

	/* tail became src_begin */ 
	dest_tail->data = src_begin->data;
	dest_tail->next = src_begin->next;

	/* src_begit becomes tail dummy of src */
	src_begin->next = NULL;
	src_begin->data = src;

	/* src_tail become dest tail dummy */ 
	src_tail->data = dest;

	/* appdeting the tails pointer */ 
	dest->tail = src->tail;
	src->tail = src_begin;

	return dest; 
}
	

