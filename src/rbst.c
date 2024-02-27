/******************************************************************************
* Project: Binart Searcing Tree                                               *
*                                                                             *
* wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by : Eli                                                            *
*                                                                             *
******************************************************************************/ 

#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert       */
#include <stddef.h>  /* size_t       */

#include "rbst.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) ((type *)malloc(sizeof(type) * nelements));
#define POSITION(status) (status/abs(status) + 1)/2;
#define OTHER_POSITION(position) (position ^ 1)
#define MAX(num_1, num_2) ((num_1) > (num_2) ? (num_1) : (num_2))

enum ilrd_status
{
	ILRD_SUCCESS =  0,
	ILRD_FAILURE = -1
};

enum position
{
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILDREN
};

struct node
{
    void *data;
    size_t height;
    node_ty *children[NUM_OF_CHILDREN];
};
 
struct rbst
{
    node_ty *root;
    cmp_func_ty cmp_func;
};


/* functions for Height */
static size_t Height(node_ty *node);
static size_t UpdateHeight(node_ty *node);

/* functions for Balancing */
static int GetBalanceFactor(node_ty *node) 
static node_ty *Rotate(node_ty *node, int direction);
static node_ty *Balance(node_ty *node);

/* functions for Destroy() function */
static void RecursiveDestroyHelper(node_ty *node);

/* functions for Insert() function */
static void RecursiveInsertHelper(node_ty *, node_ty *, cmp_func_ty);
static void InitializeNewNode(void *new_data, node_ty *new_node);

/* functions for Remove() function */
static void     NodeDataSwap(node_ty *node1, node_ty *node2);
static node_ty *DeleteNode(node_ty *node ,cmp_func_ty cmp_func, void *data);
static node_ty *GetSmallestParentInSubTree(node_ty *parent, node_ty *node);
static node_ty *GetLargestParentInSubTree(node_ty *parent, node_ty *node);

/* functions for Size() function */
static size_t RecursiveSizeHelper(node_ty *node);

/* functions for Find() function */
static node_ty *RecursiveFindHelper(node_ty *node ,cmp_func_ty, void *);

/* functions for ForEach() function*/
static int  InOrderRecursiveForEachHelper(node_ty *, operation_ty, void *);
static int  PostOrderRecursiveForEachHelper(node_ty *, operation_ty, void *);
static int  PreOrderRecursiveForEachHelper(node_ty *, operation_ty, void *);

/*******************************************************************************
************************************* Create **********************************/

rbst_ty *RBSTCreate(cmp_func_ty cmp_func)
{
	rbst_ty *new_rbst = NULL;

	assert(cmp_func); 

	new_rbst = MALLOC(1, rbst_ty)

	if(NULL == new_rbst)
	{
		return NULL;
	}

	new_rbst->cmp_func = cmp_func; 
	new_rbst->root = NULL; 

	return new_rbst;
}

/*******************************************************************************
************************************* Destroy *********************************/

void RBSTDestroy(rbst_ty *rbst)
{
	assert(NULL != rbst);

	if(!RBSTIsEmpty(rbst))
	{
		RecursiveDestroyHelper(rbst->root);
	}

	FREE(rbst);
}

static void RecursiveDestroyHelper(node_ty *node)
{
	if(NULL != node->children[LEFT])
	{
		RecursiveDestroyHelper(node->children[LEFT]);
	}

	if(NULL != node->children[RIGHT])
	{
		RecursiveDestroyHelper(node->children[RIGHT]);
	}

	FREE(node);
	return;
}

/*******************************************************************************
************************************** Size ***********************************/

size_t RBSTSize(const rbst_ty *rbst)
{
	size_t counter = 0;

	assert(NULL != rbst);

	if(RBSTIsEmpty(rbst))
	{
		return counter;
	}

	counter = RecursiveSizeHelper(rbst->root);

	return counter; 
}

static size_t RecursiveSizeHelper(node_ty *node)
{
	size_t counter = 0;

	assert(NULL != node);

	if(NULL != node->children[LEFT])
	{
		counter += RecursiveSizeHelper(node->children[LEFT]);
	}

	if(NULL != node->children[RIGHT])
	{
		counter += RecursiveSizeHelper(node->children[RIGHT]);
	}

	return ++counter;
}

/*******************************************************************************
************************************* Height **********************************/

size_t RBSTHeight(const rbst_ty *rbst)
{
	assert(NULL != rbst);

	return (Height(rbst->root) - 1); 
}

/*******************************************************************************
************************************* IsEmpty *********************************/

int RBSTIsEmpty(const rbst_ty *rbst)
{
	assert(NULL != rbst);
	
	return (NULL == rbst->root);
}

/*******************************************************************************
************************************* Insert **********************************/

int RBSTInsert(rbst_ty *rbst, void *new_data)
{
	node_ty *new_node = NULL;

	assert(NULL != rbst);

	new_node = MALLOC(1, node_ty);

	if(NULL == new_node)
	{
		return ILRD_FAILURE; 
	}

	InitializeNewNode(new_data, new_node);

	if(RBSTIsEmpty(rbst))
	{
		rbst->root = new_node;

		return ILRD_SUCCESS;
	}

	RecursiveInsertHelper(rbst->root, new_node, rbst->cmp_func);

	rbst->root = Balance(rbst->root);

	rbst->root->height = UpdateHeight(rbst->root);

	return ILRD_SUCCESS;
}

static void InitializeNewNode(void *new_data, node_ty *new_node)
{
	new_node->data = new_data;
	new_node->height = 1;
	new_node->children[LEFT]  = NULL;
	new_node->children[RIGHT]  = NULL;
}

static void RecursiveInsertHelper(node_ty *node, node_ty *new_node,
                                                           cmp_func_ty cmp_func)
{
	int cmp_status = 0;
	int position = LEFT;

	assert(NULL != node);
	assert(NULL != cmp_func);

	cmp_status = cmp_func(new_node->data, node->data);
	position = POSITION(cmp_status);

	if(NULL == node->children[position])
	{
		node->children[position] = new_node;
		node->height = UpdateHeight(node);
		return;
	}

	RecursiveInsertHelper(node->children[position], new_node, cmp_func);

	node->height = UpdateHeight(node);

	node->children[position] = Balance(node->children[position]);
}

/*******************************************************************************
************************************* Remove **********************************/

void RBSTRemove(rbst_ty *rbst, void *data)
{
	node_ty *node = NULL;
	node_ty *parent = NULL;

	int cmp_status = 0; 
	int position = 0;

	assert(NULL != rbst);

	if(RBSTIsEmpty(rbst))
	{
		return;
	}

	rbst->root = DeleteNode(rbst->root, rbst->cmp_func, data);
}

/********************************* Delete Node ********************************/

static node_ty *DeleteNode(node_ty *node ,cmp_func_ty cmp_func, void *data)
{
	int cmp_status = 0; 
	int position = 0;
	int other_position = position;

	node_ty *parent_next = NULL;
	node_ty *next = NULL;

	assert(NULL != node);
	assert(NULL != cmp_func);

	cmp_status = cmp_func(data, node->data);
	position = POSITION(cmp_status);

	if(0 < abs(cmp_status))
	{
		assert(NULL != node->children[position]);

		node->children[position] = DeleteNode(node->children[position] 
			                                                    ,cmp_func,data);
		node->height = UpdateHeight(node);

		return Balance(node);
	}

	/* case 1: the node to remove its a leaf */ 
	if(NULL == node->children[RIGHT] && NULL == node->children[LEFT])
	{
		FREE(node)
		return NULL; 
	}

	/* case 2: the node to remove have a child */ 
	if (NULL != node->children[RIGHT])
	{
		position = RIGHT;
		parent_next = GetSmallestParentInSubTree(node, node->children[position]);
	}
	else
	{
		position = LEFT;
		parent_next = GetLargestParentInSubTree(node, node->children[position]);
	} 

	other_position = position;

	if(node != parent_next)
		other_position = OTHER_POSITION(position);

	NodeDataSwap(node, parent_next->children[other_position]);

	next = parent_next->children[other_position];

	/* case 2.1: the swap node have a child */
	if(NULL != next->children[position])
	{
		parent_next->children[other_position] = next->children[position];
	}
	else
	{
		parent_next->children[other_position] = NULL;
	}

	parent_next->height = UpdateHeight(parent_next);

	FREE(next)

	return node;
}

/************************ Get Smallest Parent In Sub Tree *********************/

static node_ty *GetSmallestParentInSubTree(node_ty *parent, node_ty *node)
{
	while(NULL != node->children[LEFT])
	{
		parent = node;
		node = node->children[LEFT];
	}
		
	return parent;
}

/************************ Get Largest Parent In Sub Tree **********************/

static node_ty *GetLargestParentInSubTree(node_ty *parent, node_ty *node)
{
	while(NULL != node->children[RIGHT])
	{
		parent = node;
		node = node->children[RIGHT];
	}
		
	return parent;
}

/******************************** Node Data Swap ******************************/

static void NodeDataSwap(node_ty *node1, node_ty *node2)
{
	void *tmp = NULL;

	assert(NULL != node1);
	assert(NULL != node2);

	tmp = node1->data;
	node1->data = node2->data;
	node2->data = tmp;
		
	return;
}

/*******************************************************************************
************************************* Find ************************************/

void *RBSTFind(const rbst_ty *rbst, void *data)
{
	node_ty *node = NULL; 

	assert(NULL != rbst);

	if(RBSTIsEmpty(rbst))
	{
		return NULL;
	}

	node = RecursiveFindHelper(rbst->root, rbst->cmp_func, data);

	return node->data;
}

static node_ty *RecursiveFindHelper(node_ty *node ,cmp_func_ty cmp_func, 
	                                                                 void *data)
{
	int cmp_status = 0;
	int position = LEFT;

	assert(NULL != node);
	assert(NULL != cmp_func);

	cmp_status = cmp_func(data, node->data);
	position = POSITION(cmp_status);

	if(0 == cmp_status)
	{
		return node;
	}

	if(NULL == node->children[position])
	{
		return NULL;
	}

	return RecursiveFindHelper(node->children[position], cmp_func, data);
}

/*******************************************************************************
*********************************** For Each **********************************/

int RBSTForEach(rbst_ty *rbst, operation_ty op, void *param, trav_ty trav_order)
{
	int status = ILRD_SUCCESS;

    switch(trav_order)
	{	
		case(POST_ORDER):
			status = InOrderRecursiveForEachHelper(rbst->root, op, param);
			break;

		case(In_ORDER):
			status = PostOrderRecursiveForEachHelper(rbst->root, op, param);
			break;

		case(PRE_ORDER):
			status = PreOrderRecursiveForEachHelper(rbst->root, op, param);
			break;

		default:
			break;
	}	
	
	return status;
}

/*********************************** In Order *********************************/

static int InOrderRecursiveForEachHelper(node_ty* node, operation_ty op,
                                                                    void *param)
{
	int status = ILRD_SUCCESS;

	if(node == NULL)
	{
		return status;
	}
	status = InOrderRecursiveForEachHelper(node->children[LEFT], op, param);
	if(status != ILRD_SUCCESS) return status;

	status = op(node->data, param);
	if(status != ILRD_SUCCESS) return status;

	return(InOrderRecursiveForEachHelper(node->children[RIGHT], op, param));
}

/********************************** Post Order ********************************/

static int PostOrderRecursiveForEachHelper(node_ty* node, operation_ty op, 
	                                                                void *param)
{
	int status = ILRD_SUCCESS;

	if(node == NULL)
	{
		return status;
	}

	status = PostOrderRecursiveForEachHelper(node->children[LEFT], op, param);
	if(status != ILRD_SUCCESS) return status;

	status = PostOrderRecursiveForEachHelper(node->children[RIGHT], op, param);
	if(status != ILRD_SUCCESS) return status;

	return(op(node->data, param));
}

/********************************** Pre Order *********************************/

static int PreOrderRecursiveForEachHelper(node_ty* node, operation_ty op, 
	                                                                void *param)
{
	int status = ILRD_SUCCESS;

	if(node == NULL)
	{
		return status;
	}

	status = op(node->data, param);
	if(status != ILRD_SUCCESS) return status; 

	status = PreOrderRecursiveForEachHelper(node->children[LEFT], op, param);
	if(status != ILRD_SUCCESS) return status;

	return (PreOrderRecursiveForEachHelper(node->children[RIGHT], op, param));
}

/*******************************************************************************
************************************ Height ***********************************/

static size_t Height(node_ty *node)
{
	if(NULL == node)
	{
		return 0;
	}

	return(node->height);
}

static size_t UpdateHeight(node_ty *node)
{	
	return (MAX(Height(node->children[LEFT]),Height(node->children[RIGHT])) +1);
}


/*******************************************************************************
*********************************** Balance ***********************************/

static node_ty *Balance(node_ty *node)
{
    int balance_factor = 0;
    int balance_factor_child = 0;
    int position = 0;
    int o_position = 0; 

    balance_factor = GetBalanceFactor(node);
    position = POSITION(balance_factor);
    o_position = OTHER_POSITION(position); 

    if (1 < abs(balance_factor))
    {
    	balance_factor_child = GetBalanceFactor(node->children[o_position]);

    	if(0 > balance_factor_child * balance_factor)
    	{
    		node->children[o_position] = Rotate(node->children[o_position],
    		                                                        o_position);
    	}
    	return(Rotate(node, position));
    }
	return node;
}

/***************************** Get Balance Factor *****************************/

static int GetBalanceFactor(node_ty *node) 
{
    return Height(node->children[LEFT]) - Height(node->children[RIGHT]);
}

/********************************** Rotate ************************************/

static node_ty *Rotate(node_ty *node, int direction)
{
    node_ty *new_root = NULL;
    node_ty *new_root_child = NULL;

    int other_direction = OTHER_POSITION(direction);

    new_root = node->children[other_direction];
    new_root_child = new_root->children[direction];

    new_root->children[direction] = node;
    node->children[other_direction] = new_root_child;
 
    node->height = UpdateHeight(node);
    new_root->height = UpdateHeight(new_root);

    return new_root;
}
 





