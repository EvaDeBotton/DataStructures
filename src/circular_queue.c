/******************************************************************************
* Project: Circular Queue                                                     *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Bnaya                                                            *
*                                                                             *
******************************************************************************/



/******************************************************************************
*************************** Circular Queue Create ****************************/

/* 1) 	allocating a maintained struct, that will save the current 
        read_ptr, and write_ptr.
   2)   allocate array of n length. 
   3) 	set both read_ptr, and write_ptr to the begining of the array. 

struct circular_queue
{
	iter_ty read_ptr; 
	iter_ty write_ptr; 
};

*/ 

/******************************************************************************
************************** Circular Queue Destroy ****************************/

/* 
    1) free the array  
    2) free the maintained struct

*/  

/******************************************************************************
*************************** Circular Queue Write *****************************/

/* 
	1) 	ceacking if the circular queue is full, if full return fail status. 
		cheking : if the write_ptr bigger then read_ptr by one. 
		(write_ptr == (read_ptr + 1) % length) ? full; 
    2)  writing in to the courrent write_ptr position.
    3) 	incress the adddres of courrent write_ptr by one. 
		incresment by one: 
		write_ptr = (write_ptr + 1) % length; 

*/ 

/******************************************************************************
*************************** Circular Queue Read ******************************/

/* 
    1)  ceacking if the circular queue is empty, if empty return fail status. 
		cheking : if the write_ptr equal to read_ptr. 
		(write_ptr == read_ptr) ? empty; 
    2)  reading from the courrent read_ptr position.
    3) 	incress the adddres of courrent read_ptr by one. 
		incresment by one: 
		read_ptr = (read_ptr + 1) % length; 

*/

/******************************************************************************
************************ Circular Queue Free Space ***************************/

/* 
    1) is_empty -> if empty return length;  
    2) if write_ptr bigger then read_ptr, return (write_ptr + length - read_ptr) 
    3) if read_ptr bigger then write_ptr, return (read_ptr - write_ptr) 

*/  





