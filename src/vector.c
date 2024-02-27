/******************************************************************************
* vector                                                                      *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Liel                                                             *
*                                                                             *
******************************************************************************/
#include<stdlib.h> /* malloc free */
#include <stdio.h> /* puts, printf */
#include <assert.h> /* assert */
#include "../include/vector.h"

#define SUCSSES                       0
#define FAILURE                       1
#define GF                            2 
#define SHRINK_FACTOR            1/(GF)
#define QUARTER               1/(GF*GF)

#define IS_EMPTY(vector) (vector->size == 0) ? 1 : 0
#define IS_FULL(vector)  (vector->size == vector->capacity) ? 1 : 0

struct vector
{
	void **array;   
	size_t size;
	size_t capacity;
	size_t init_capacity;
};

static status_ty ReallocVector(vector_ty *vector, size_t new_capacity);

/*******************************************************************************
******************************* Vector Create *********************************/

vector_ty *VectorCreate(size_t capacity)
{
	vector_ty *vector = (vector_ty *)malloc(sizeof(vector_ty));

	if(NULL == vector)
	{
		puts("fail to malloc vector");
		return NULL; 
	}

	vector->array = (void **)malloc(sizeof(void*) * capacity);

	if(NULL == vector->array)
	{
		puts("fail to malloc vector");
		free(vector);
		vector = NULL;

		return NULL;
	} 

	vector->size = 0;
	vector->capacity = capacity;
	vector->init_capacity = capacity;
	
	return vector;
}

/*******************************************************************************
********************************* Vector Destroy ******************************/

void VectorDestroy(vector_ty *vector)
{
	assert(NULL != vector);

	free(vector->array);
	vector->array = NULL;
	
	free(vector);
	vector = NULL; 
}

/*******************************************************************************
********************************** Vector Size ********************************/

size_t VectorSize(const vector_ty *vector)
{
	assert(NULL != vector);

	return (vector->size);
} 


/*******************************************************************************
******************************** Vector Capacity ******************************/

size_t VectorCapacity(const vector_ty *vector)
{
	assert(NULL != vector);

	return (vector->capacity);
}

/*******************************************************************************
***************************** Vector Shrink To Fit ****************************/

status_ty VectorShrinkToFit(vector_ty *vector)
{
	assert(NULL != vector);
	
	if(SUCSSES == ReallocVector(vector, vector->size)) 
	{
		vector->init_capacity = vector->size;
		
		return SUCSSES;
	}
	
	return FAILURE; 
} 

/*******************************************************************************
******************************** Vector Get Val *******************************/

void *VectorGetVal(const vector_ty *vector, size_t index)
{
	assert(NULL != vector);

	return(vector->array[index]);
}

/*******************************************************************************
******************************** Vector Set Val *******************************/

void *VectorSetVal(vector_ty *vector, size_t index, void *element)
{
	assert(NULL != vector);

	vector->array[index] = element;
	
	return (vector->array[index]);
}

/*******************************************************************************
******************************* Vector Push Back ******************************/

status_ty VectorPushBack(vector_ty *vector, void *element)
{
	assert(NULL != vector);

	if(0 == vector->capacity)
	{
		vector->capacity = 1; 
	}
	 
	if(0 != IS_FULL(vector))
	{
		if(FAILURE == ReallocVector(vector, (vector->capacity * GF)))
		{
			return FAILURE;
		}
	}

	vector->array[vector->size] = element;
	++vector->size;
		
	return SUCSSES;
}

/*******************************************************************************
******************************* Vector Pop Back *******************************/

status_ty VectorPopBack(vector_ty *vector)
{
	assert(NULL != vector);
	assert(0 == IS_EMPTY(vector));

	--vector->size;
	
	if((vector->size <= (vector->capacity * QUARTER)) && 
		           ((vector->capacity * SHRINK_FACTOR) > vector->init_capacity))
	{ 
		return (ReallocVector(vector, vector->capacity / GF));
	}
	
	return SUCSSES; 
}

/*******************************************************************************
****************************** Vector Reserve *********************************/

status_ty VectorReserve(vector_ty *vector, size_t new_capacity)
{
	assert(NULL != vector); 
	
	if(vector->capacity < new_capacity) 
	{
		if(FAILURE == ReallocVector(vector, new_capacity)) 
		{
			return FAILURE;
		}

		vector->init_capacity = vector->capacity;
	}

	return SUCSSES; 
}

/*******************************************************************************
****************************** Vector New Size ********************************/

static status_ty ReallocVector(vector_ty *vector, size_t new_capacity)
{
	void *tmp_allocation = vector->array;

	assert(NULL != vector); 
	
	tmp_allocation = realloc(vector->array, new_capacity * sizeof(void*));
	
	if(tmp_allocation == NULL)
	{
		puts("fail to realloc vector");
		return FAILURE;
	}
	
	vector->array = tmp_allocation;
	vector->capacity = new_capacity;
	
	return SUCSSES;
}

/******************************************************************************/


