/*******************************************************************************
* Description: Header of functions
* Name: HRD20
* Date: 25/08/21
* Versions: 1.0 ready for review
*******************************************************************************/
#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

/* 	 function that compares data1 to data2 and return:							*
 *			positive number if data1 > data2)									*
 *			zero if data1 == data 2												*
 *			negative number if data1 < data2									*/
typedef int (*cmp_func_ty)(const void *data1, const void *data2);

/*   function that checks if the user_data matches the data of the pqueue_data	*
 *   and returns 1 if they match, otherwise returns 0							*/
typedef int (*is_match_ty)(const void *pqueue_data, const void *user_data);

/* action_func_ty is a function that applies an operation on chosen elements    *
 * the return value is 0 for success, 1 otherwise                               */
typedef int (*operations_ty)(void *data, void *param);

#endif /* __FUNCTIONS_H__ */