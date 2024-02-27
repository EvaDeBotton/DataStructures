/******************************************************************************
* lut mirror                                                                  *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Ariel                                                            *
*                                                                             *
******************************************************************************/

#include <stdio.h> /* printf, fopen */
#include <stdlib.h> /* system, exit*/

#define MAX_U_CHAR 128

void InitializeLutMirrorInFile(const char *file_name);
unsigned char BitsArrMirror(unsigned char bit_array);
static int AppendBitArrayToTheEnd(unsigned char bit_arr, const char *file_name);

/******************************************************************************
******************************** The Main  ***********************************/

int main ()
{  
	InitializeLutMirrorInFile("lut_mirror_table");
	
  	return 0; 
}
/*******************************************************************************
*********************** Initialize Lut Mirror In File *************************/

void InitializeLutMirrorInFile(const char *file_name)
{
	unsigned char bit_arr = 0;
	unsigned char bit_arr_mirrored = 0; 
	
	while(bit_arr < MAX_U_CHAR)
	{
		bit_arr_mirrored = BitsArrMirror(bit_arr);
		AppendBitArrayToTheEnd(bit_arr_mirrored, file_name);
		bit_arr = bit_arr + 1;  
	}
}


/*******************************************************************************
****************************** Bits Arr Mirror ********************************/
/*  The BitsArrMirror() function 
*/
unsigned char BitsArrMirror(unsigned char bit_array)
{
    bit_array = ((bit_array & 0x55) << 1 ) | ((bit_array & 0xaa) >> 1); 
    bit_array = ((bit_array & 0x33) << 2 ) | ((bit_array & 0xcc) >> 2);
    bit_array = ((bit_array & 0x0f) << 4 ) | ((bit_array & 0xf0) >> 4);

	return bit_array;  
}

/*****************************************************************************
************************* Append String To The End ***************************/

static int AppendBitArrayToTheEnd(unsigned char bit_arr, const char *file_name)
{
	FILE *file_ptr = NULL;
	
    /*  Open for appending (writing at end of file) */ 
    file_ptr = fopen(file_name, "a"); 
    
    /*  Appending string_from_user at the end of the file */
    fprintf(file_ptr,"%u,", bit_arr);
    
    /*  closing the file */ 
    fclose(file_ptr); 

    return 0;
}


