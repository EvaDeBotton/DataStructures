/******************************************************************************
* bit array test                                                              *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Arial                                                            *
*                                                                             *
******************************************************************************/

#include "../include/bit_array.h" 
#include "../include/lut_mirror_table.h" 
#include "../include/lut_set_on_table.h"
#include "../include/defines.h"

#include <stddef.h> /* size_t */
#include <stdio.h>  /* puts   */
#include <assert.h> /* assert */

#define CHAR_BIT          8
#define SIZE_T_BITS       64



/*******************************************************************************
****************************** Bits Arr Set All *******************************/

/*  The BitsArrSetAll() function 
*/
bit_arr_ty BitsArrSetAll(bit_arr_ty bit_array)
{	
	return MASK_ALL_ON;  
}

/*******************************************************************************
****************************** Bits Arr Reset All *****************************/

/*  The BitsArrResetAll() function
*/
bit_arr_ty BitsArrResetAll(bit_arr_ty bit_array)
{
	return MASK_ALL_OFF;  
}

/*******************************************************************************
******************************* Bits Arr Set On *******************************/

/*  The BitsArrSetOn() function 
*/
bit_arr_ty BitsArrSetOn(bit_arr_ty bit_array, size_t position)  
{	
	assert(position < SIZE_T_BITS);

	return (bit_array ^ ((size_t)1 << position));  
}

/*******************************************************************************
****************************** Bits Arr Set Off *******************************/

/*  The BitsArrSetOff() function
*/
bit_arr_ty BitsArrSetOff(bit_arr_ty bit_array, size_t position)
{
	assert(position < SIZE_T_BITS);

	return (bit_array ^ ((size_t)1 << position));  
}

/*******************************************************************************
******************************* Bits Arr Set Bit ******************************/
/*  The BitsArrSetBit() function
*/
bit_arr_ty BitsArrSetBit(bit_arr_ty bit_array, size_t position,size_t bit_state)
{
	assert(position < SIZE_T_BITS);
	
	bit_array = BitsArrSetOff(bit_array,position);
	
	return (bit_array | (bit_state << position)); 
}

/*******************************************************************************
****************************** Bits Arr Get Val *******************************/
/*  The BitsArrGetVal() function 
*/
size_t BitsArrGetVal(bit_arr_ty bit_array, size_t position)
{
	assert(position < SIZE_T_BITS);
	
	return ((bit_array & ((size_t)1 << position)) >> position);  
}

/*******************************************************************************
******************************* Bits Arr Flip *********************************/

/*  The BitsArrFlip() function
*/
bit_arr_ty BitsArrFlip(bit_arr_ty bit_array, size_t position) 
{	
	assert(position < SIZE_T_BITS);

	return bit_array ^ ((size_t)1 << position);  
}

/*******************************************************************************
*************************** Bits Arr Mirror Witout Lut ************************/

/*  The BitsArrMirror() function 
*/

bit_arr_ty BitsArrMirror(bit_arr_ty bit_array)
{
	bit_arr_ty mask = MASK_5555;
	
    bit_array = (((bit_array & mask)) << ST1)|((bit_array & (mask << ST1)) >> ST1); 
    
    mask = MASK_3333;
    bit_array = (((bit_array & mask)) << ST2)|((bit_array & (mask << ST2)) >> ST2);
    
    mask = MASK_0F0F;
    bit_array = (((bit_array & mask)) << ST4)|((bit_array & (mask << ST4)) >> ST4);
    
    mask = MASK_00FF;
    bit_array = (((bit_array & mask)) << ST8)|((bit_array & (mask << ST8)) >> ST8);
      
    mask = MASK_00FF2;
    bit_array = (((bit_array & mask)) << ST16)|((bit_array & (mask << ST16)) >> ST16);
    
    mask = MASK_00FF4;
    bit_array = (((bit_array & mask)) << ST32)|((bit_array & (mask << ST32)) >> ST32);

	return bit_array;  
}

/*******************************************************************************
****************************** Bits Arr Mirror ********************************/

/*  The BitsArrMirror() function 
*/

bit_arr_ty BitsArrMirror2(bit_arr_ty bit_array)
{
	char num_of_bytes = sizeof(bit_arr_ty);
	bit_arr_ty mirrored_arr = 0;
	 
	while(0 < num_of_bytes)
	{
		mirrored_arr |= (bit_arr_ty)mirored_byte_lut[bit_array & MASK_BYTE_ON];
		mirrored_arr <<= CHAR_BIT;
		bit_array    >>= CHAR_BIT;
		--num_of_bytes;
	}
	
	return mirrored_arr;  
}

/*******************************************************************************
******************************* Bits Arr Rot R ********************************/
/*  The BitsArrRotR() function
*/
bit_arr_ty BitsArrRotR(bit_arr_ty bit_array, size_t num_of_rot)
{
	return (bit_array << num_of_rot)|(bit_array >> (SIZE_T_BITS - num_of_rot));
}

/*******************************************************************************
******************************* Bits Arr Rot L ********************************/

/*  The BitsArrRotL() function
*/
bit_arr_ty BitsArrRotL(bit_arr_ty bit_array, size_t num_of_rot)
{
	return (bit_array >> num_of_rot)|(bit_array << (SIZE_T_BITS - num_of_rot));
}

/*******************************************************************************
****************************** Bits Arr Count On ******************************/

/*  The BitsArrCountOn() function
*/

size_t BitsArrCountOn(bit_arr_ty bit_array)
{
	size_t counter = 0;
	 
	while(0 != bit_array)
	{
		bit_array = bit_array & (bit_array - 1); 
		counter++; 
	}
	return counter; 
}
/*
size_t BitsArrCountOn(bit_arr_ty bit_array)
{
	bit_arr_ty mask = MASK_5555;
	
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST1 )  >>  ST1);
	
	mask = MASK_3333;
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST2 )  >>  ST2); 
	
	mask = MASK_0F0F;
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST4 )  >>  ST4); 
	
	mask = MASK_00FF;
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST8 )  >>  ST8); 
	
	mask = MASK_00FF2;
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST16 ) >> ST16);
	
	mask = MASK_00FF4;
	bit_array = ((bit_array & mask)) + (bit_array & (mask <<  ST32 ) >> ST32); 
	     
	return bit_array; 
}*/
/******************************************************************************
************************* Count Num Of Bit Witout Loop ************************/

size_t BitsArrCountOn2(bit_arr_ty bit_array)
{
	bit_arr_ty counter = 0;
	char num_of_bytes = sizeof(bit_arr_ty);
	
	while(0 < num_of_bytes)
	{
		counter  = BitsSetTable256[ bit_array & MASK_BYTE_ON];
		bit_array = bit_array >>  CHAR_BIT;
		--num_of_bytes;
	}
	
	return counter;
}
 
/*******************************************************************************
***************************** Bits Arr Count Off ******************************/

/*  The BitsArrCountOff() function
*/
size_t BitsArrCountOff(bit_arr_ty bit_array)
{
	return  SIZE_T_BITS - BitsArrCountOn(bit_array);  
}

/*******************************************************************************
***************************** Bits Arr To String ******************************/

/*  The BitsArrToString() function
*/
char *BitsArrToString(bit_arr_ty bit_array, char *dest)
{
	char *start_of_dest = dest;
	size_t position = 62;
	char Lut[2][1] = {"0","1"};
	
	while(position > 0)
	{
		*dest = *Lut[BitsArrGetVal(bit_array,position)]; 
		++dest;
		--position; 
	}
	
	return start_of_dest;  
}




