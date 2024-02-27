/******************************************************************************
* string test .c                                                              *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Arial                                                            *
*                                                                             *
******************************************************************************/
#include "../include/bit_array.h"
#include <stdio.h> /*size_t */

#define CHAR_BIT          8
#define SIZE_T_BITS       sizeof(bit_arr_ty) * CHAR_BIT

void BitsArrSetAllTest(void);
void BitsArrSetOnTest(void);
void BitsArrGetValTest(void);
void BitsArrRotTest(void);
void BitsArrCountTest(void);
void BitsArrResetAllTest(void);
void BitsArrSetOffTest(void);
void BitsArrFlipTest(void);
void BitsArrToStringTest(void);
void BitsArrSetBitTest(void);
void BitsArrMirrorTest(void);
void EndOfTests(void);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  
	BitsArrSetAllTest();
	BitsArrResetAllTest();
	BitsArrSetOnTest();
	BitsArrSetOffTest();
	BitsArrSetBitTest();
	BitsArrGetValTest();
	BitsArrFlipTest();
	BitsArrMirrorTest();
	BitsArrRotTest();
	BitsArrCountTest();
	BitsArrToStringTest();
	EndOfTests();
	
	return 0; 
}

/*******************************************************************************
**************************** Bits Arr Set All Test ****************************/

void BitsArrSetAllTest(void)
{
	bit_arr_ty number =  123456; 
	bit_arr_ty all_bit_set = 0xffffffffffffffff;
	
	puts("\n\n_________________ Bits Arr Set All Test _______________________");
	
	if ( all_bit_set == BitsArrSetAll(number))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}

}

/*******************************************************************************
*************************** Bits Arr Reset All Test ***************************/

void BitsArrResetAllTest(void)
{
	bit_arr_ty number =  123456; 
	bit_arr_ty all_bit_reset = 0x0000000000000000;
	
	puts("\n\n_________________ Bits Arr Reset All Test _____________________");
	
	if ( all_bit_reset == BitsArrResetAll(number))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}

}

/*******************************************************************************
***************************** Bits Arr Set On Test ****************************/

void BitsArrSetOnTest(void)
{
	size_t position = 2; 
	bit_arr_ty bit_array =  0x0000000000000000; 
	bit_arr_ty one_bit_set = 0x0000000000000004;

	puts("\n\n___________________ Bits Arr Set On Test ______________________");
	
	if(one_bit_set == BitsArrSetOn(bit_array, position) )
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}

/*******************************************************************************
***************************** Bits Arr Set Off Test ***************************/

void BitsArrSetOffTest(void)
{
	size_t position = 0; 
	bit_arr_ty bit_array =  0xffffffffffffffff; 
	bit_arr_ty one_bit_off = 0xfffffffffffffffe;

	puts("\n\n___________________ Bits Arr Set Off Test _____________________");
	
	if(one_bit_off == BitsArrSetOff(bit_array, position) )
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}

/*******************************************************************************
***************************** Bits Arr Set Bit Test ***************************/

void BitsArrSetBitTest(void)
{
	size_t position = 0;
	bit_arr_ty bit_state = 0; 
	bit_arr_ty bit_array =  0xffffffffffffffff; 
	bit_arr_ty one_bit_off = 0xfffffffffffffffe;

	puts("\n\n___________________ Bits Arr Set Bit Test _____________________");
	
	if(one_bit_off == BitsArrSetBit(bit_array, position,bit_state))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}


/*******************************************************************************
***************************** Bits Arr Get Val Test ***************************/

void BitsArrGetValTest(void)
{
	size_t position = 31;

	bit_arr_ty bit_array =  0x00000000f0000000; 

	puts("\n\n___________________ Bits Arr Get Val Test _____________________");
	
	if(1 == BitsArrGetVal(bit_array, position))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}


/*******************************************************************************
******************************** Bits Arr Flip Test ***************************/

void BitsArrFlipTest(void)
{
	bit_arr_ty bit_array =  0x0000000000000F00;
	bit_arr_ty bit_fliped = 0x0000000000000000;
	 
	bit_array = BitsArrFlip(bit_array, 8);
	bit_array = BitsArrFlip(bit_array, 9);
	bit_array = BitsArrFlip(bit_array,10);
	bit_array = BitsArrFlip(bit_array,11);

	puts("\n\n_____________________ Bits Arr Flip Test ______________________");
	
	if(bit_fliped == bit_array)
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}

/*******************************************************************************
***************************** Bits Arr Mirror Test ****************************/

void BitsArrMirrorTest(void)
{
	bit_arr_ty bit_array_before =  0x00000000abcdef12;
	bit_arr_ty bit_array =  0x00000000abcdef12;
	bit_array = BitsArrMirror(bit_array);
	bit_array = BitsArrMirror(bit_array);
	puts("\n\n___________________ Bits Arr Mirror Test ______________________");
	
	if(bit_array_before == bit_array)
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}


/*******************************************************************************
******************************** Bits Arr Rot Test ****************************/

void BitsArrRotTest(void)
{
	size_t num_of_rot = 8; 
	bit_arr_ty bit_array_before =  0x00000000abcdef12;
	bit_arr_ty bit_array =  0x00000000abcdef12;
	 
	bit_array = BitsArrRotL(bit_array,num_of_rot);
	bit_array = BitsArrRotR(bit_array,num_of_rot);

	puts("\n\n_____________________ Bits Arr Rot Test _______________________");
	
	if(bit_array_before == bit_array)
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}

/*******************************************************************************
****************************** Bits Arr Count Test ****************************/

void BitsArrCountTest(void)
{
	size_t num_of_set_on_bits = 16; 
	bit_arr_ty bit_array =  0x1111111111111111;

	puts("\n\n_________________ Bits Arr Count Set on Test __________________");
	
	if(num_of_set_on_bits == BitsArrCountOn(bit_array))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
	
	puts("\n\n________________ Bits Arr Count Set off Test __________________");
	
	if((SIZE_T_BITS - BitsArrCountOn(bit_array)) == BitsArrCountOff(bit_array))
	{
		printf("\n Test no %d / %d was sucssesful !",1, 1);
	}
	else 
	{
		printf("\n Test no %d / %d fail !", 1, 1);
	}
}

/*******************************************************************************
***************************** Bits Arr To String Test *************************/

void BitsArrToStringTest(void)
{
	char dest[1][65] = {""}; 
	bit_arr_ty bit_array =  0x1111111111111111;
	
	puts("\n\n__________________ Bits Arr To String Test ____________________");
	
	BitsArrToString(bit_array, *dest);
	
	puts(*dest);
}

/*******************************************************************************
******************************* End of Tests **********************************/

void EndOfTests(void)
{  
	puts("\n\n_____________________________________________________________\n");
}

