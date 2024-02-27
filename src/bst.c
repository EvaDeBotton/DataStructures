/******************************************************************************
* Project: Binart Searcing Tree                                               *
*                                                                             *
* wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by : Liel                                                         *
*                                                                             *
*                                                                             *
******************************************************************************/ 

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert       */
#include <stddef.h> /* size_t       */

#include "bst.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) ((type *)malloc(sizeof(type) * nelements));
#define POSITION(status) (status/abs(status) + 1)/2;

#define IS_EQUAL    0
#define NONVALID    0xDEADBEAF

enum ilrd_status
{
	ILRD_SUCCESS = 0,
	ILRD_FAILURE = 1
};

enum position
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};

struct node
{
    void *data;
    node_ty *children[NUM_OF_CHILDREN];
    node_ty *parent; 
};
 
struct bst
{
    node_ty pre_root;
    cmp_func_ty cmp_func;
};


static bst_iter_ty GetFirsRightParent(bst_iter_ty iter);
static bst_iter_ty GetFirstLeftParent(bst_iter_ty iter);
static bst_iter_ty GetSmallestInSubTree(bst_iter_ty iter);
static bst_iter_ty GetLargestInSubTree(bst_iter_ty iter);
static int ChildPosition(bst_iter_ty runner, bst_iter_ty runner_parent);
static void AttachNodes(bst_iter_ty iter_san, bst_iter_ty iter_parent);
static void MoveNextToNodePosition(bst_iter_ty next, bst_iter_ty node);
static void BSTSwap(bst_iter_ty iter, bst_iter_ty next);

/*******************************************************************************
****************************** Binary Tree Create *****************************/

bst_ty *BSTCreate(cmp_func_ty cmp_func)
{
	bst_ty *new_bst = NULL; 
	bst_iter_ty node = NULL;

	assert(cmp_func); 

	/* Allocate memory for new_bst */
	new_bst = MALLOC(1, bst_ty)

	if(NULL == new_bst)
	{
		return NULL;
	}

	new_bst->cmp_func = cmp_func; 

	/* initialize the pre root node */ 
	node = &(new_bst->pre_root);

	node->data = (void *)NONVALID;
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	node->parent = NULL;

	return new_bst;
}

/*******************************************************************************
****************************** Binary Tree Insert *****************************/

bst_iter_ty BSTInsert(bst_ty *bst, void *new_data)
{	
	bst_iter_ty node_runner = NULL;
	bst_iter_ty new_node = NULL;

	int cmp_status = 0;
	int position = LEFT; 
	
	assert(NULL != bst);

	node_runner = &(bst->pre_root);

	while(NULL != node_runner->children[position])
	{
		node_runner = node_runner->children[position];
		cmp_status = bst->cmp_func(new_data, node_runner->data);	
		position = POSITION(cmp_status);
	}  

	new_node = MALLOC(1, node_ty);

	if(NULL == new_node)
	{
		return &(bst->pre_root); 
	}

	node_runner->children[position] = new_node;

	/* Initialize the new node */
	new_node->data = new_data;
	new_node->children[RIGHT] = NULL;
	new_node->children[LEFT]  = NULL;
	new_node->parent = node_runner;

	return new_node;
}

/*******************************************************************************
*********************************** BST Destroy *******************************/

void BSTDestroy(bst_ty *bst)
{
	bst_iter_ty runner = NULL;
	bst_iter_ty runner_parent = NULL;
	bst_iter_ty pre_root = NULL; 

	int position = 0;

	assert(NULL != bst);

	pre_root = &(bst->pre_root);

	if(NULL != pre_root->children[LEFT])
	{
		runner = GetSmallestInSubTree(pre_root->children[LEFT]);

		while(pre_root != runner)
		{
   			if(NULL != runner->children[RIGHT])
   			{
   				runner = GetSmallestInSubTree(runner->children[RIGHT]);
   			}
   			else
   			{
   				position = ChildPosition(runner, runner_parent);

   				runner_parent->children[position] = NULL; 

   				FREE(runner);
 
   				runner = runner_parent;
   			}

   			runner_parent = runner->parent;
   		}
	}

	FREE(bst); 


}

/*******************************************************************************
************************************* BST Size ********************************/

size_t BSTSize(const bst_ty *bst)
{
	bst_iter_ty pre_root = NULL;
	bst_iter_ty runner = NULL;  

	size_t counter = 0;

	assert(NULL != bst);

	pre_root = (bst_iter_ty)&(bst->pre_root);
	
   	runner = BSTBegin(bst);

   	while(pre_root != runner)
   	{
   		runner = BSTNext(runner);
   		++counter;
   	}

   	return counter;
}

/*******************************************************************************
********************************** BST Is Empty *******************************/

int BSTIsEmpty(const bst_ty *bst)
{
	assert(bst);
	
	return (bst->pre_root.children[LEFT] == NULL);
}

/*******************************************************************************
************************************ BST Remove *******************************/
/*
void BSTRemove(bst_iter_ty iter)
{
	int position = 0;

	bst_iter_ty next_parent = NULL;
	bst_iter_ty parent = NULL;
	bst_iter_ty next = NULL;

	assert(NULL != iter);

	parent = iter->parent;
	position = ChildPosition(iter, parent);
*/
	/* case 1: the node to remove its a leaf */ 
	/*if(NULL == iter->children[RIGHT] && NULL == iter->children[LEFT])
	{
		parent->children[position] = NULL; 

		FREE(iter)
		return; 
	}	*/

	/* case 2: the node to remove have a child */ 
	/*if(NULL != iter->children[RIGHT])
	{
		position = RIGHT;
		next = GetSmallestInSubTree(iter->children[position]);
	}
	else
	{
		position = LEFT;
		next = GetLargestInSubTree(iter->children[position]);
	}  */

	/* case 2.1: the next node have a child */ 
/*	if(NULL != next->children[position])
	{
		next_parent = next->parent;
		position = ChildPosition(next, next_parent);
	  	AttachNodes(next->children[position], next_parent);
	}

	MoveNextToNodePosition(next, iter);
	FREE(iter)

 	return;  
}*/

void BSTRemove(bst_iter_ty iter)
{
	bst_iter_ty next_parent = NULL;
	bst_iter_ty parent = NULL;
	bst_iter_ty next = NULL;

	int position = 0;

	assert(NULL != iter);

	parent = iter->parent;
	position = ChildPosition(iter, parent);

	/* case 1: the node to remove its a leaf */ 
	if(NULL == iter->children[RIGHT] && NULL == iter->children[LEFT])
	{
		parent->children[position] = NULL; 

		FREE(iter)
		return; 
	}
	

	/* case 2: the node to remove have a child */ 
	if (NULL != iter->children[RIGHT])
	{
		position = RIGHT;
		next = GetSmallestInSubTree(iter->children[position]);
	}
	else
	{
		position = LEFT;
		next = GetLargestInSubTree(iter->children[position]);
	} 

	BSTSwap(iter, next); 

	if(NULL != iter->children[position])
	{
	  	AttachNodes(iter->children[position], iter->parent);
	}

	FREE(iter)

}



/*******************************************************************************
************************************ BST Begin ********************************/

bst_iter_ty BSTBegin(const bst_ty *bst)
{
	bst_iter_ty root  = NULL;
	bst_iter_ty begin = NULL;
	bst_iter_ty pre_root = NULL;   

	assert(NULL != bst);

	pre_root =  &(((bst_ty *)bst)->pre_root);
	root = pre_root->children[LEFT];

	if(NULL == pre_root->children[LEFT])
   	{
   		return pre_root;
   	}

   	begin = GetSmallestInSubTree(root);
	
	return(begin);
}

/*******************************************************************************
************************************ BST End **********************************/

bst_iter_ty BSTEnd(const bst_ty *bst)
{
	assert(NULL != bst);

	return ((bst_iter_ty)&(bst->pre_root));
}

/*******************************************************************************
************************************ BST Prev *********************************/

bst_iter_ty BSTPrev(const bst_iter_ty iter)
{
	if(NULL != iter->children[LEFT])
	{
		return GetLargestInSubTree(iter->children[LEFT]);
	}
	else
	{	
		return GetFirstLeftParent(iter);
	}
}

/*******************************************************************************
************************************ BST Next *********************************/

bst_iter_ty BSTNext(const bst_iter_ty iter)
{
	if(NULL != iter->children[RIGHT])
	{
		return GetSmallestInSubTree(iter->children[RIGHT]);
	}
	else
	{	
		return GetFirsRightParent(iter);
	}
}

/*******************************************************************************
******************************** BST Iter Is Equal ****************************/

int BSTIterIsEqual(const bst_iter_ty iter1, const bst_iter_ty iter2)
{
	return (iter1 == iter2); 
}

/*******************************************************************************
********************************** BST Get Data *******************************/

void *BSTGetData(const bst_iter_ty iter)
{
	assert(NULL != iter);

	return iter->data; 
}

/*******************************************************************************
*********************************** BST Find **********************************/

bst_iter_ty BSTFind(const bst_ty *bst, void *new_data)
{
   	bst_iter_ty pre_root = NULL;
	bst_iter_ty runner = NULL;

	int cmp_status = 0;
	int position = 0; 
	
	assert(NULL != bst);

	pre_root = (bst_iter_ty)&(bst->pre_root);

	runner = pre_root->children[LEFT];

	if(NULL == runner)
	{
		return pre_root;
	}
	
	cmp_status  = bst->cmp_func(new_data, runner->data);

	while(IS_EQUAL != cmp_status)
	{
		position = POSITION(cmp_status)

		if(NULL == runner->children[position])
		{
			return pre_root;
		}

		runner = runner->children[position];
		cmp_status = bst->cmp_func(new_data, runner->data);
	} 

	return runner;
}

/*******************************************************************************
********************************** BST For Each *******************************/

int BSTForEach(bst_iter_ty from, const bst_iter_ty to, operation_ty op,
	                                                                void *param)
{
	bst_iter_ty runner = NULL;

	int status = ILRD_SUCCESS;  

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != op);

	runner = from; 

	while(to != runner && ILRD_SUCCESS == status)
    {
   		status = op(runner->data, param); 
   		runner = BSTNext(runner);
	}

	return status;
}

/******************************************************************************/

static bst_iter_ty GetSmallestInSubTree(bst_iter_ty iter)
{
	while(NULL != iter->children[LEFT])
	{
		iter = iter->children[LEFT];
	}
		
	return iter;
}

static bst_iter_ty GetLargestInSubTree(bst_iter_ty iter)
{
	while(NULL != iter->children[RIGHT])
	{
		iter = iter->children[RIGHT];
	}
		
	return iter;
}

static bst_iter_ty GetFirsRightParent(bst_iter_ty iter)
{
	while(RIGHT == ChildPosition(iter, iter->parent))
	{
		iter = iter->parent;
	}

	return iter->parent;
}

static bst_iter_ty GetFirstLeftParent(bst_iter_ty iter)
{
	while(LEFT == ChildPosition(iter, iter->parent))
	{
		iter = iter->parent;
	}

	return iter->parent;
}

static int ChildPosition(bst_iter_ty runner, bst_iter_ty runner_parent)
{
	if(runner == runner_parent->children[LEFT])
	{
		return LEFT;
	}
	else
	{
		return RIGHT;
	}
}

static void AttachNodes(bst_iter_ty iter_child, bst_iter_ty iter_parent)
{ 
	int position = ChildPosition(iter_child, iter_parent);

	if(position == RIGHT)
	{
		position = LEFT;
	}
	else 
	{
		position = RIGHT;
	}

	iter_parent->children[position] = iter_child;

	iter_child->parent = iter_parent;
}

static void MoveNextToNodePosition(bst_iter_ty next, bst_iter_ty node)
{
	int position = ChildPosition(node, node->parent);

	bst_iter_ty node_right_child = (bst_iter_ty)&(node->children[RIGHT]);
	bst_iter_ty node_left_child  = (bst_iter_ty)&(node->children[LEFT]);
	bst_iter_ty node_parent = (bst_iter_ty)&(node->parent);

	node_parent->children[position] = next;

	next->children[RIGHT] = node_right_child;
	node_right_child->parent = next;
	
	next->children[LEFT] = node_left_child;
	node_left_child->parent = next;

	next->parent = node_parent;
 
}

static void BSTSwap(bst_iter_ty iter, bst_iter_ty next)
{
	enum position iter_as_child_pos = 0;
	enum position next_as_child_pos = 0;

	bst_iter_ty iter_left = iter->children[LEFT];
	bst_iter_ty iter_right = iter->children[RIGHT];
	bst_iter_ty iter_parent = iter->parent;
	
	bst_iter_ty next_left = next->children[LEFT];
	bst_iter_ty next_right = next->children[RIGHT];
	bst_iter_ty next_parent = next->parent;
	
	iter_as_child_pos = ChildPosition(iter, iter_parent);
	next_as_child_pos = ChildPosition(next, next_parent);
	
	if (next_parent == iter)
	{
		next->children[RIGHT] = iter;
		iter->parent = next;
	}
	else
	{
		next->children[RIGHT] = iter_right;
		iter->parent = next_parent;
		next_parent->children[next_as_child_pos] = iter;
		if (iter_right) iter_right->parent = next;
	}
	
	iter->children[LEFT] = next_left;
	iter->children[RIGHT] = next_right;
	
	next->children[LEFT] = iter_left;
	next->parent = iter_parent;
	
	iter_parent->children[iter_as_child_pos] = next;
	
	if (iter_left) iter_left->parent = next;
	
	if (next_left) next_left->parent = iter;
	if (next_right) next_right->parent = iter;
}


