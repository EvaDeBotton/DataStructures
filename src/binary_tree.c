/******************************************************************************
* Project: TREE BINARY                                                        *
*                                                                             *
* wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by :                                                                *
*                                                                             *
*                                                                             *
******************************************************************************/ 

#include <stdio.h>  /* puts   */
#include <stddef.h> /* NULL   */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#define DEADBEEF           0xDEADBEEFBAADF00D
enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) ((type *)malloc(sizeof(type) * nelements));
#define POSITION(status) (status/abs(status) + 1)/2;

typedef struct bst bst_ty;
typedef struct node node_ty;
typedef node_ty *bst_iter_ty;

typedef int (* cmp_func_ty)(void* data1, void *data2);

enum position
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};

enum compare
{
    SMALLER = -1,
    EQUAL = 0,
    BIGGER = 1
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


bst_ty *BSTCreate(cmp_func_ty cmp_func);

bst_iter_ty BSTInsert(bst_ty *bst, void *new_data);

static bst_iter_ty GetSmallestInSubTree(bst_iter_ty iter);

static int ChildPosition(bst_iter_ty runner, bst_iter_ty runner_parent);

static bst_iter_ty GetSubLeftTreeParent(bst_iter_ty iter);

bst_iter_ty BSTNext(const bst_iter_ty iter);

size_t BSTSize(const bst_ty *bst);

void BSTDestroy(bst_ty *bst);

int int_cump(void* data, void *param)
{
	if(*(int*)data > *(int*)param)
	{
		return BIGGER;
	}
	else if(*(int*)data < *(int*)param)
	{
		return SMALLER;
	}

	return EQUAL;
}

int main()
{
	bst_ty *bst = BSTCreate(int_cump);

	size_t size = 0; 

	int num1 = 5;
	int num2 = 1;
	int num3 = 3;
	int num4 = 7;
	int num5 = 4;
	int num6 = 2; 

	void *data = &num1; 
	BSTInsert(bst, data);

	data = &num2; 
	BSTInsert(bst, data);

	data = &num3; 
	BSTInsert(bst, data);

	data = &num4; 
	BSTInsert(bst, data);

	data = &num5; 
	BSTInsert(bst, data);

	data = &num6; 
	BSTInsert(bst, data);

	size = BSTSize(bst);

	printf("\n%lu\n",size);

	BSTDestroy(bst);

	size = BSTSize(bst);

	printf("\n%lu\n",size);

	return 0; 

}

/*******************************************************************************
****************************** Binary Tree Create *****************************/

bst_ty *BSTCreate(cmp_func_ty cmp_func)
{
	bst_ty *bst = NULL; 
	bst_iter_ty node = NULL; 

	bst = MALLOC(1, bst_ty)

	if(NULL == bst)
	{
		return NULL;
	}

	assert(cmp_func);

	bst->cmp_func = cmp_func; 

	node = &(bst->pre_root);

	node->data = NULL;
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	node->parent = NULL;

	return bst;
}

/*******************************************************************************
****************************** Binary Tree Insert *****************************/

bst_iter_ty BSTInsert(bst_ty *bst, void *new_data)
{	
	bst_iter_ty node_runner = NULL;
	bst_iter_ty new_node = NULL;

	int cmp_status = 0;
	int position = 0; 
	
	assert(NULL != bst);

	new_node = MALLOC(1, node_ty)

	if(NULL == new_node)
	{
		return NULL; 
	}

	new_node->data = new_data;
	new_node->children[RIGHT] = NULL;
	new_node->children[LEFT]  = NULL;

	node_runner = &(bst->pre_root);

	if(NULL == node_runner->children[LEFT])
	{
		node_runner->children[LEFT] = new_node;
		new_node->parent = node_runner;
		return new_node;
	}

	node_runner = node_runner->children[LEFT];
	cmp_status  = bst->cmp_func(new_data, node_runner->data);

	while(EQUAL != cmp_status)
	{
		position = POSITION(cmp_status)

		if(NULL == node_runner->children[position])
		{
			node_runner->children[position] = new_node;
			new_node->parent = node_runner;

			return new_node;
		}

		node_runner = node_runner->children[position];
		cmp_status = bst->cmp_func(new_data, node_runner->data);
	} 

	return &(bst->pre_root); 
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

   		FREE(runner)
 
   		runner = runner_parent;
   	}

   	runner_parent = runner->parent;
   }
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

   if(NULL == pre_root->children[LEFT])
   {
   	return counter;
   }

   runner = GetSmallestInSubTree(pre_root->children[LEFT]);

   while(pre_root != runner)
   {
   	runner = BSTNext(runner);
   	++counter;
   }

   return counter;
}

/*******************************************************************************
************************************ BST Remove *******************************/

void BSTRemove(bst_iter_ty iter);
{
	int position = 0;

	bst_iter_ty next_parent = NULL;
	bst_iter_ty parent = NULL;
	bst_iter_ty next = NULL;

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
	if(NULL != iter->children[RIGHT])
	{
		child_1 = RIGHT;
		next = GetSmallestInSubTree(node->children[child_1])
	}
	else
	{
		child_1 = LEFT;
		next = GetLargstInSubTree(node->children[child_1])
	}  

	next_parent = next->parent;
	position = ChildPosition(next, next_parent);

	/* case 2.1: the next node have a child */ 
	if(NULL != next->children[child_1])
	{
	  	AttachNodes(next->children[child_1], next_parent);
	}

	MoveNextToNodePosition(next, node);

 	return;  
}



/*******************************************************************************
************************************ BST Begin ********************************/

bst_iter_ty BSTBegin(const bst_ty *bst)
{
	bst_iter_ty root = NULL;  

   assert(NULL != bst);

	root = bst->pre_root->children[LEFT];
	
	return(GetSmallestInSubTree(root));
}

/*******************************************************************************
************************************ BST End **********************************/

bst_iter_ty BSTEnd(const bst_ty *bst)
{
   return ((bst_iter_ty)&(bst->dummy_node));
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
		return GetSubRightTreeParent(iter);
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
		return GetSubLeftTreeParent(iter);
	}
}

/*******************************************************************************
******************************** BST Iter Is Equal ****************************/

int BSTIterIsEqual(const bst_iter_ty iter1, const bst_iter_ty iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2); 
}

/*******************************************************************************
********************************** BST Get Data *******************************/

void *BSTGetData(const bst_iter_ty iter)
{
	return iter->data; 
}

/*******************************************************************************
*********************************** BST Find **********************************/

bst_iter_ty BSTFind(const bst_ty *bst, void *data)
{
   bst_iter_ty pre_root = NULL;
	bst_iter_ty runner = NULL;

	int cmp_status = 0;
	int position = 0; 
	
	assert(NULL != bst);

	pre_root = (bst_iter_ty)&(bst->pre_root);

	runner = pre_root;

	if(NULL == runner->children[LEFT])
	{
		return pre_root;
	}

	runner = runner->children[LEFT];
	cmp_status  = bst->cmp_func(new_data, runner->data);

	while(EQUAL != cmp_status)
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

   assert(NULL != from);
   assert(NULL != to);

   runner = from; 

   while(to != runner && SUCSSES == status)
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

static bst_iter_ty GetSubLeftTreeParent(bst_iter_ty iter)
{
	while(RIGHT == ChildPosition(iter, iter->parent))
	{
		iter = iter->parent;
	}

	return iter->parent;
}

static bst_iter_ty GetSubRightTreeParent(bst_iter_ty iter)
{
	while(LEFT == ChildPosition(iter, iter->parent))
	{
		iter = iter->parent;
	}

	return iter->parent;
}



   		
