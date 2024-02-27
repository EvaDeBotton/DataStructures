/******************************************************************************
* linked_list_exercises                                                       *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Doron                                                            *
*                                                                             *
******************************************************************************/
#include <assert.h> /* assert */
#include <stddef.h> /* NULL */
#include "linked_list_exercises.h"

struct node 
{
	void *data;
	struct node *next; 

};

/* helper function */

static node_t *CountAndReturnLestNode(node_t *head_1, int *count);
static node_t *GetIntesectionNode(int d, node_t * head_bigger, 
	                                                    node_t * head_smaller);


/******************************************************************************
************************************* Flip ***********************************/
/* 
_______________________________  BEFORE ______________________________________

 ---->     A   ---->       B    ---->       C     ---->      D        ----> 
ptr_to_A      ptr_to_B         ptr_to_C          ptr_to_D          ptr_to_NULL

________________________________  AFTER ______________________________________

 <----     A   <----       B    <----       C     <----      D        <---- 
ptr_to_NULL    ptr_to_A         ptr_to_B          ptr_to_C           ptr_to_D 

The iterator should be advanced two times

*/
node_t *Flip(node_t *head)
{
    node_t *iter_1 = NULL; 
    node_t *iter_2 = NULL;
    node_t *iter_3 = NULL; 

    assert(NULL != head);

	iter_1 = head;

    if(NULL == iter_1->next)
    {
        return iter_1; 
    } 

	iter_2 = head->next;

    if(NULL == iter_2->next)
    {
        iter_2->next = iter_1; 
        
        return iter_2; 
    } 

	iter_3 = head->next->next; 

	head->next = NULL;

	while(iter_3->next != NULL)
	{
		 iter_2->next = iter_1;

		 iter_1 = iter_2;
		 iter_2 = iter_3; 
		 iter_3 = iter_3->next; 
	}

	iter_2->next = iter_1; 
	iter_3->next = iter_2; 

	return iter_3; 
}

/******************************************************************************
*********************************** Has Loop *********************************/

int HasLoop(const node_t *head)
{
    const node_t *iter_1 = head;
    const node_t *iter_2 = head;

    assert(NULL != head);

	while (NULL != iter_2->next)
    {
        iter_1 = iter_1->next;
        if(NULL == iter_2->next->next) return 0;

        iter_2 = iter_2->next->next;       

        if(iter_1 == iter_2) return 1;
    }

    return 0;
}

/******************************************************************************
****************************** Find Intersection *****************************/

node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
    int count_1 = 0; 
    int count_2 = 0;
    int diff = 0; 

    node_t *tail_1 = NULL; 
    node_t *tail_2 = NULL;
    node_t *head_bigger = NULL; 
    node_t *head_smaller = NULL;
    node_t *intersection_node = NULL;

    assert(NULL != head_1); 
    assert(NULL != head_2); 

    tail_1 = CountAndReturnLestNode(head_1, &count_1); 
    tail_2 = CountAndReturnLestNode(head_2, &count_2); 

    /* check if has intersection */
    if(tail_1 != tail_2)
    {
        return NULL; 
    }

    /* determine parameters for GetIntesectionNode function */
    if(count_1 >= count_2)
    {
        diff = count_1 - count_2; 
        head_bigger = head_1; 
        head_smaller = head_2;
    }
    else
    {
        diff = count_2 - count_1; 
        head_bigger = head_2; 
        head_smaller = head_1;
    }

    intersection_node = GetIntesectionNode(diff, head_bigger, head_smaller);

    return intersection_node; 
}

/******************** static - Count And Return Lest Node ********************/

/* Takes head pointer of the linked list and pointer to count, and return the 
   last node iter and the noumber of node in the list. */

static node_t *CountAndReturnLestNode(node_t *head, int *count)
{
	node_t *iter = head; 
    *count = 0;
 
    while (iter->next != NULL) 
    {
        (*count)++;
        iter = iter->next;
    }
 
    return iter;
}

/*********************** static - Get Intesection Node ***********************/

/* function to get the intersection point of two linked lists head_bigger and 
   head_smaller where head_bigger has d more nodes than head_smaller */

static node_t *GetIntesectionNode(int diff, node_t *head_bigger,
                                                         node_t *head_smaller)
{
	node_t *iter_1 = head_bigger; 
    node_t *iter_2 = head_smaller; 

    while(0 != diff)
    {
        iter_1 = iter_1->next;
        --diff; 
    }
 
    while (iter_1->next != iter_2->next) 
    {
        iter_1 = iter_1->next;
        iter_2 = iter_2->next;
    }
 
    return iter_1->next;
}



