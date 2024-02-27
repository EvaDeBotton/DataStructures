#ifndef __UTILS_H__
#define __UTILS_H__

/* colors for debugging */
#define RED			                    "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"
#define WHITE                         "\033[0;37m"

#define ON                1
#define OFF               0
#define BITS_IN_BYTE      8

#define FREE(ptr) {free(ptr); ptr = NULL;} 
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);
#define MAX(num_1, num_2) ((num_1) > (num_2) ? (num_1) : (num_2));

enum is_empty 
{
   NOT_EMPTY = 0, 
   EMPTY  = 1 
};

enum return_status 
{
   ILRD_SUCCESS = 0, 
   ILRD_FAILURE = 1 
};

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum occupied_status 
{ 
   ILRD_FREE = 0,
   ILRD_OCCUPIED = 1,
   ILRD_RESERVED = 2
};

enum is_equal 
{ 
   ILRD_EQUAL = 0,
   ILRD_NOT_EQUAL = 1
};

enum position
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};


#endif /* __UTILS_H__ */