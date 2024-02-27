/******************************************************************************
* Project: SORTING ALGORITHMS                                                 *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd by Philip                                                           *
*                                                                             *
******************************************************************************/ 

#include "utils.h"

#include <assert.h>  /* assert */
#include <stdlib.h>
#include <stdio.h>


#include "sorting.h"

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);

void RadixSort(unsigned int *unsorted_arr, size_t arr_size, 
               unsigned int exponent_of_two);

static void HelperCountingSort(unsigned int *begin_of_arr,
                                unsigned int *buffer, 
                                size_t arr_size, unsigned int *histogram,
                                size_t histogram_size, unsigned int round, 
                                unsigned int num_of_bits, 
                                unsigned int offset);

static unsigned int CreateMask(unsigned int num_of_bits, unsigned int round);

static void HistogramFromIntArr( unsigned int *histogram,
                                 unsigned int *begin_of_arr, 
                                 unsigned int *end_of_arr, unsigned int mask, 
                                 unsigned int offset);

static void DeterminePositionRange(unsigned int *histogram_begin,
                                                         size_t histogram_size);

static void FillSortedArrPosition(unsigned int *histogram, 
                              unsigned int *unsorted_arr, 
                              unsigned int *end_arr, unsigned int *sorted_arr, 
                              unsigned int mask, unsigned int offset);

/*******************************************************************************
************************************ The Main  ********************************/
void PrintBit(unsigned int number)
{
    char number_bit[32];
    int i = 0;
    unsigned int remainder = 0;

    for(i = 0; i < 32; i++)
    {
        number_bit[i] = '0';
    }

    for(i = 0; i < 32; i++)
    {
        remainder = number % 2;
        if(remainder == 1)
            number_bit[31 - i] = '1';
        number /= 2;
    }

    for(i = 0; i < 32; i++)
    {
        printf("%c ", number_bit[i]);

    }

}

int main ()
{  
    unsigned int  arr_size = 20;
    unsigned int  exponent_of_two = 4;  
    unsigned int *unsorted_arr = MALLOC(arr_size, unsigned int)
    unsigned int i = 0;
    int my_rand;

    for(i = 0; i < arr_size; i++)
    {
        my_rand = rand() % 1000;
        unsorted_arr[i] = (unsigned int)(my_rand); 
        printf("%u\n", unsorted_arr[i]);
    }

    RadixSort(unsorted_arr, arr_size, exponent_of_two);


    for(i = 0; i < arr_size; i++)
    {
        /*printf("\n %u \n", unsorted_arr[i]); 
        PrintBit(unsorted_arr[i]); */
    }
    
    return 0; 
}

/******************************************************************************
********************************** Radix Sort *********************************/

void RadixSort(unsigned int *unsorted_arr, size_t arr_size, 
               unsigned int exponent_of_two)
{
    unsigned int  num_of_bits = 1 << exponent_of_two;
    unsigned int  histogram_size = 1 << num_of_bits;

    unsigned int *buffer = MALLOC(arr_size, unsigned int)
    unsigned int *histogram = MALLOC(histogram_size, unsigned int)
    unsigned int *tmp_ptr = NULL; 

    unsigned int  round = 1;
    unsigned int  offset = 0; 

    unsigned int  num_of_rounds = 32 / num_of_bits;

    unsigned int i = 0;


    for(round = 1; round < num_of_rounds + 1 ;round++)
    {
        offset = (round - 1)*num_of_bits;

        HelperCountingSort( unsorted_arr, buffer, arr_size, histogram, 
                            histogram_size, round, num_of_bits, offset);

        tmp_ptr = unsorted_arr;
        unsorted_arr = buffer;
        buffer = tmp_ptr;

        /*for(i = 0; i < arr_size; i++)
            unsorted_arr[i] = buffer[i];*/ 
        for(i = 0; i < histogram_size; i++)
            histogram[i] = 0;
   
    }

    for(i = 0; i < arr_size; i++)
    {
        printf("\n %u \n", unsorted_arr[i]); 
        PrintBit(unsorted_arr[i]);
    }

    FREE(buffer)
    FREE(histogram)


}


/******************************************************************************
******************************** Counting Sort ********************************/


static void HelperCountingSort(unsigned int *begin_of_arr,
                                unsigned int *buffer, 
                                size_t arr_size, unsigned int *histogram,
                                size_t histogram_size, unsigned int round, 
                                unsigned int num_of_bits, 
                                unsigned int offset)
{
    unsigned int *end_of_arr = NULL; 
    unsigned int mask = 0;

    assert(NULL != begin_of_arr);

    end_of_arr = begin_of_arr + (arr_size - 1);

    /* STEP 1 : create a mask acording to the round and num_of_bits. */
    mask = CreateMask(num_of_bits, round);

    /* STEP 2 : Computing a histogram of the number of times each number
                occurs within the arrary.  */
    HistogramFromIntArr(histogram, begin_of_arr, end_of_arr, mask, offset);

    /* STEP 3 : Performs a prefix sum computation on a count in order to 
                determine, for each element, the position range where the 
                element should be placed. */
    DeterminePositionRange(histogram, histogram_size);

    /* STEP 4 : loops over the items of the array, in reversed order 
                (to preserve stability), moving each item into its sorted 
                position in the output array */
    FillSortedArrPosition(histogram, begin_of_arr, end_of_arr, buffer, mask, offset);

}

/*******************************************************************************
******************** static function - Non comparison sort ********************/

/**************************** Find Histogram Size *****************************/

/*static int FindHistogramSize(int *begin_of_arr, int* end_of_arr)
{    
    int *min_element = begin_of_arr;
    int *max_element = begin_of_arr;
    int *runner = begin_of_arr;

    assert(NULL != begin_of_arr);
    assert(NULL != end_of_arr);

    while(runner <= end_of_arr) 
    { 
        if ( *runner > *max_element ) 
        {
            max_element = runner;
        }
        else if( *runner < *min_element ) 
        {
            min_element = runner;
        }

        ++runner;
    }

    offset = *min_element;

    return ( *max_element - *min_element + 1 );
} */


/*************************** Histogram From Int Arr ***************************/

static unsigned int CreateMask(unsigned int num_of_bits, unsigned int round)
{
    unsigned int mask = 0;
    unsigned int i = 0;

    /* set on first num_of_bits number of bit */
    for(i = 1; i < num_of_bits; i++)
    {
        ++mask;
        mask = mask << 1;

    }

    ++mask;
    puts("  ");

    /* Moving the position of the on bits to the current mask position */
    mask = mask << ((round - 1) * num_of_bits);

    PrintBit(mask);

    return mask; 
}

/*************************** Histogram From Int Arr ***************************/

static void HistogramFromIntArr( unsigned int *histogram,
                                 unsigned int *begin_of_arr, 
                                 unsigned int *end_of_arr, unsigned int mask, 
                                 unsigned int  offset)
{
    unsigned int *runner = begin_of_arr; 
    unsigned int  position = 0;

    assert(NULL != begin_of_arr);
    assert(NULL != end_of_arr);



    while(runner <= end_of_arr)
    {
        position = (*runner & mask) >> offset;
        ++histogram[position];
        ++runner; 
    }
}

/************************** Determine Position Range **************************/

static void DeterminePositionRange(unsigned int *histogram_begin,
                                                          size_t histogram_size)
{
    unsigned int *histogram_runner = histogram_begin;
    unsigned int *histogram_end = NULL;

    assert(NULL != histogram_begin);

    histogram_end = histogram_begin + (histogram_size - 1);

    while(histogram_runner < histogram_end)
    {
        ++histogram_runner; 
       *histogram_runner += *(histogram_runner -1);
    }
}

/**************************** Fill Sorted Position ****************************/

static void FillSortedArrPosition(unsigned int *histogram, 
                                  unsigned int *unsorted_arr, 
                                  unsigned int *end_arr, unsigned int *sorted_arr, 
                                  unsigned int mask, unsigned int offset)
{
    unsigned int *runner = NULL;
    unsigned int position = 0;

    assert(NULL != histogram);
    assert(NULL != unsorted_arr);
    
    runner = end_arr; 

    while(runner >= unsorted_arr)
    {   
        position = (*runner & mask) >> offset;
        histogram[position] -= 1;

        sorted_arr[histogram[position]] = *runner;

        --runner;
    }
}

