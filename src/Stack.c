/******************************************************************************
* Stack                                                                       *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Nisan Ohana                                                      *
*                                                                             *
******************************************************************************/
#include<stdlib.h> /* malloc free */
#include <assert.h> /* assert */
#include "Stack.h" 

struct stack
{
	void **array;   
	size_t size;
	size_t capacity;
};


/*******************************************************************************
******************************** Stack Create *********************************/

stack_ty *StackCreate(size_t capacity)
{
	stack_ty *stack = (stack_ty *)malloc(sizeof(stack_ty)); 
	stack->array = malloc(sizeof(void*) * capacity);
	stack->size = 0;
	stack->capacity = capacity;
	
	return stack;
}

/*******************************************************************************
******************************* Stack Destroy *********************************/

void StackDestroy(stack_ty *stack)
{
	free(stack->array);
	free(stack);
}

/*******************************************************************************
********************************** Stack Push *********************************/

void StackPush(stack_ty *stack, void *element)
{
	assert(stack->capacity > stack->size);
	stack->array[stack->size] = element;
	++stack->size;
}


/*******************************************************************************
********************************** Stack Pop **********************************/

void StackPop(stack_ty *stack)
{
	--stack->size;
	stack->array[stack->size] = NULL;
}

/*******************************************************************************
******************************** Stack Peek ***********************************/

void *StackPeek(const stack_ty *stack)
{
	return(stack->array[stack->size - 1]);
}

/*******************************************************************************
********************************** Stack Size *********************************/

size_t StackSize(const stack_ty *stack)
{
	return (stack->size);
}

/*******************************************************************************
******************************** Stack Capacity *******************************/

size_t StackCapacity(const stack_ty *stack)
{
	return (stack->capacity);
}


/*******************************************************************************
******************************* Stack Is Empty ********************************/

bool_ty StackIsEmpty(const stack_ty *stack)
{
	return (stack->size == 0 ? 1 : 0);
}
