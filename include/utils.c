#ifndef __UTILS_H__
#define __UTILS_H__

/* colors for debugging */
#define RED			                     "\033[0;31m"
#define GREEN 			                 "\033[0;32m"
#define YELLOW			                 "\033[0;33m"


enum status 
{
	SUCSSES = 0, 
	FAILURE = 1 
}; 

enum is_successful 
{
	ILRD_FALSE = 0, 
	ILRD_TRUE  = 1 
};

enum is_empty 
{
	NOT_EMPTY = 0, 
	EMPTY  = 1 
};

enum is_equal 
{
	NOT_EQUAL = 0, 
	EQUAL  = 1 
};


#endif /* __UTILS_H__ */