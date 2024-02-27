/******************************************************************************
* Project: SORTING ALGORITHMS                                                 *
*                                                                             *
* wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by : Philip - comparsion sort                                       *
*              Nisan  - non comparsion sort                                   *
*                                                                             *
*                                                                             *
******************************************************************************/ 
#include <assert.h>  /* assert        */
#include <stdlib.h>  /* malloc,  free */
#include <string.h>  /* memcpy */

#include "sorting.h"

#include <stdio.h>

enum status 
{
    SUCSSES = 0, 
    FAILURE = 1 
}; 

/******************************* macro function *******************************/ 

#define FREE(ptr) {free(ptr); ptr = NULL;}
#define MALLOC(nelements, type) (type *)malloc(sizeof(type) * nelements);
#define CALLOC(nelements, type) (type *)calloc(nelements, sizeof(type));
#define POSITION(current) ((current) - (offset));

#define SYSTEM_FAILER    -1


/********************* static function - comparison sort **********************/

static void Swap(int *num_1, int *num_2);
static void AdvanceByOne(int *num);
static int *FindMinElementInArr(int *begin, int* end);


/******************* static function - Non comparison sort ********************/

static void DeterminePositionRange(int *histogram_begin, size_t histogram_size);
static void HistogramFromIntArr(   int *histogram,    
                                   int *begin_of_arr,   int *end_of_arr);
static  int FillSortedPosition(    int *histogram, 
                                   int *begin_of_arr,   int *end_of_arr);
static  int FindHistogramSize(     int *begin_of_arr,   int *end_of_arr);
static void CopyArr(int *src_arr,  int *begin_dest_arr, int *end_dest_arr);
static void HelperCountingSort(unsigned int *begin_of_arr,
                                unsigned int *buffer, 
                                size_t arr_size, unsigned int *histogram,
                                size_t histogram_size, unsigned int round, 
                                unsigned int num_of_bits, 
                                unsigned int offset);
static unsigned int CreateMask(unsigned int num_of_bits, unsigned int round);
static void HistogramFromIntArrRadix( unsigned int *histogram,
                                 unsigned int *begin_of_arr, 
                                 unsigned int *end_of_arr, unsigned int mask, 
                                 unsigned int offset);
static void DeterminePositionRangeRadix(unsigned int *histogram_begin,
                                                         size_t histogram_size);
static void FillSortedArrPositionRadix(unsigned int *histogram, 
                              unsigned int *unsorted_arr, 
                              unsigned int *end_arr, unsigned int *sorted_arr, 
                              unsigned int mask, unsigned int offset);
static void SetHistogramToZero(unsigned int *histogram, 
                               unsigned int histogram_size);

/*********************** static function - Merge Sort *************************/

static void MergeSortHelper(int *arr_to_sort, size_t arr_size, int* tmp_array);

static void MergeTwoSides(int *right_side_arr, int right_size,
                          int *left_side_arr,  int left_size, int *tmp_array);

/*********************** static function - Quick Sort *************************/

static void *GetPivot(void *base, size_t num_of_elements, size_t elem_size,
                                                          cmp_func_ty cmp_func);
static void *ReorderAccordingPivotValue(void *base, size_t num_of_elements, 
                                        size_t elem_size, cmp_func_ty cmp_func, 
                                         void *pivot_ptr);

static void SwapData(void *elem_1, void *elem_2, size_t elem_size);


/****************************** static variable *******************************/

static int offset = 0; 

/*******************************************************************************
********************************* Bubble Sort *********************************/

void BubbleSort(int *begin_of_arr, size_t arr_size)
{
    int *end_of_unsorted = NULL; 
    int *runner = NULL; 

    assert(NULL != begin_of_arr);

    end_of_unsorted = begin_of_arr + (arr_size - 1); 

    while(end_of_unsorted != begin_of_arr)
    {   
        runner = begin_of_arr;

        /* swapping between every two elements that are not in the correct
           size order  */ 
        while(runner != end_of_unsorted)
        {
            if (*runner > *(runner + 1))
            {
                Swap(runner, runner + 1);            
            }

            ++runner;
        }

        /*  after each loop, the end of the unsorted elements part, is 
            at the end of the last swapping  */
        end_of_unsorted = runner - 1;
    }
    
}

/*******************************************************************************
******************************** Insertion Sort *******************************/

void InsertionSort(int *begin_of_arr, size_t arr_size)
{
    int *begin_of_unsorted = NULL; 
    int *end_of_unsorted = NULL;

    int *runner = NULL; 
    int next_to_sort = 0;

    assert(NULL != begin_of_arr);

    begin_of_unsorted = begin_of_arr + 1;
    end_of_unsorted = begin_of_arr + (arr_size - 1);

    while(end_of_unsorted != begin_of_unsorted - 1) 
    {
        next_to_sort = *begin_of_unsorted;
        runner = begin_of_unsorted - 1;

        /*  advancing all the sorted elements which are bigger than the next 
            element to sort, to the next following place in the array */
        while (runner >= begin_of_arr && *runner > next_to_sort)
        {
            AdvanceByOne(runner);
            --runner;  
        }

        ++begin_of_unsorted; 

        /* moving the next element to its sorted place in the sorted part */
        ++runner;
        *(runner) = next_to_sort;

    }
}

/*******************************************************************************
******************************** Selection Sort *******************************/

void SelectionSort(int *begin_of_arr, size_t arr_size)
{
    int *begin_of_unsorted = begin_of_arr; 
    
    int *end_of_unsorted = NULL;
    int *next_min_element = NULL;

    assert(NULL != begin_of_arr);

    end_of_unsorted = begin_of_arr + (arr_size - 1); 

    while(begin_of_unsorted != end_of_unsorted)
    {
        /* find the next smaller element in the unsorted part of the array */
        next_min_element = FindMinElementInArr(begin_of_unsorted, end_of_unsorted); 

        /* moving the next smaller element to the end of the sorted part */
        Swap(begin_of_unsorted, next_min_element);
        
        /* advancing by one the start of the unsorted part*/
        ++begin_of_unsorted;
    }
}

/*******************************************************************************
********************* static function - comparison sort ***********************/

/********************************* static swap ********************************/

static void Swap( int *num_1, int *num_2)
{
    int tmp = *num_1;
 
    *num_1 = *num_2;
    *num_2 = tmp; 
}

/******************************* advance by one *******************************/

static void AdvanceByOne(int *num)
{
    *(num + 1) = *num;
}

/************************** Find Min Element In Arr ***************************/

static int *FindMinElementInArr(int *begin_of_arr, int* end_of_arr)
{    
    int *runner = begin_of_arr;
    int *min_element = begin_of_arr;

    assert(NULL != begin_of_arr);
    assert(NULL != end_of_arr);

    while(runner <= end_of_arr) 
    { 
        if (*runner < *min_element) 
        {
            min_element = runner;
        }
        ++runner;
    }

    return min_element;
}
 
/*******************************************************************************
******************************** Counting Sort ********************************/

void CountingSort(int *begin_of_arr, size_t arr_size)
{
    int *end_of_arr = NULL; 
    int *histogram  = NULL;

    int histogram_size = 0;
    int status = 0;  

    assert(NULL != begin_of_arr);

    end_of_arr = begin_of_arr + ( arr_size - 1 ); 


    /* STEP 1 : Determine the histogram size according to the scale of number 
                in the array.  */
    histogram_size = FindHistogramSize( begin_of_arr, end_of_arr ); 
    histogram = CALLOC( histogram_size, int)

    if(NULL == histogram)
    {
        return;
    }

    /* STEP 2 : Computing a histogram of the number of times each number
                occurs within the arrary.  */
    HistogramFromIntArr( histogram, begin_of_arr, end_of_arr );


    /* STEP 3 : Performs a prefix sum computation on a count in order to 
                determine, for each element, the position range where the 
                element should be placed. */
    DeterminePositionRange( histogram, histogram_size );

    /* STEP 4 : loops over the items of the array, in reversed order 
                (to preserve stability), moving each item into its sorted 
                position in the output array */
    status = FillSortedPosition( histogram, begin_of_arr, end_of_arr );

    if(SYSTEM_FAILER == status)
    {
        FREE( histogram)
        return;
    }

    FREE( histogram)

}

/*******************************************************************************
******************** static function - Non comparison sort ********************/

/**************************** Find Histogram Size *****************************/

static int FindHistogramSize(int *begin_of_arr, int* end_of_arr)
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
}

/*************************** Histogram From Int Arr ***************************/

static void HistogramFromIntArr( int *histogram,    
                                 int *begin_of_arr,   int *end_of_arr)
{
    int *runner = begin_of_arr; 
    int  position = 0;

    assert(NULL != begin_of_arr);
    assert(NULL != end_of_arr);

    while(runner <= end_of_arr)
    {
        position = POSITION(*runner)
        ++histogram[position];
        ++runner; 
    }
}

/************************** Determine Position Range **************************/

static void DeterminePositionRange(int *histogram_begin, size_t histogram_size)
{
    int *histogram_runner = histogram_begin;
    int *histogram_end = NULL;

    assert(NULL != histogram_begin);

    histogram_end = histogram_begin + (histogram_size - 1);

    while(histogram_runner < histogram_end)
    {
        ++histogram_runner; 
       *histogram_runner += *(histogram_runner -1);
    }
}

/**************************** Fill Sorted Position ****************************/

static int FillSortedPosition(int *histogram, int *unsorted_arr, int *end_arr)
{
    int *sorted_arr = NULL;
    int *runner = NULL;
    int position = 0; 
    int arr_size = 0; 

    assert(NULL != histogram);
    assert(NULL != unsorted_arr);

    arr_size = end_arr - unsorted_arr + 1;
    sorted_arr = MALLOC(arr_size, int)
    
    if(NULL == sorted_arr)
    {
        return SYSTEM_FAILER; 
    }
    
    runner = end_arr; 

    while(runner >= unsorted_arr)
    {   
        position = POSITION(*runner)
        histogram[position] -= 1;

        sorted_arr[histogram[position]] = *runner;

        --runner;
    }

    CopyArr(sorted_arr, unsorted_arr, end_arr); 

    FREE(sorted_arr)

    return 0;
}

/********************************** Copy Arr **********************************/

static void CopyArr(int *src_arr, int *dest_arr, int *end_dest_arr)
{
    while(end_dest_arr >= dest_arr)
    {
        *dest_arr= *src_arr;
        ++src_arr;
        ++dest_arr;
    }
}

/******************************************************************************
********************************** Radix Sort *********************************/

void RadixSort(unsigned int *unsorted_arr, size_t arr_size, 
               unsigned int exponent_of_two)
{
    unsigned int  round = 1;
    unsigned int  offset = 0; 

    /* Initialize the size variables */
    unsigned int  num_of_bits = 1 << exponent_of_two;
    unsigned int  histogram_size = 1 << num_of_bits;

    unsigned int *buffer = MALLOC(arr_size, unsigned int)
    unsigned int *histogram = MALLOC(histogram_size, unsigned int)

    unsigned int *tmp_ptr = NULL;

    unsigned int  num_of_rounds = 32 / num_of_bits;

    printf("num_of_bits = %u", num_of_bits);
    printf("round = %u", num_of_rounds);

    for(round = 1; round < num_of_rounds + 1 ;round++)
    {
        SetHistogramToZero(histogram, histogram_size);

        /* calculating the first bit offset */ 
        offset = (round - 1)*num_of_bits;

        HelperCountingSort( unsorted_arr, buffer, arr_size, histogram, 
                            histogram_size, round, num_of_bits, offset);
        /* swap pointers */
        tmp_ptr = unsorted_arr;
        unsorted_arr = buffer;
        buffer = tmp_ptr;
    }

    FREE(buffer)
    FREE(histogram)
}

/*******************************************************************************
******************** static function - Non comparison sort ********************/

/*************************** Helper Counting Sort *****************************/

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
    HistogramFromIntArrRadix(histogram, begin_of_arr, end_of_arr, mask, offset);

    /* STEP 3 : Performs a prefix sum computation on a count in order to 
                determine, for each element, the position range where the 
                element should be placed. */
    DeterminePositionRangeRadix(histogram, histogram_size);

    /* STEP 4 : loops over the items of the array, in reversed order 
                (to preserve stability), moving each item into its sorted 
                position in the output array */
    FillSortedArrPositionRadix(histogram, begin_of_arr, end_of_arr, buffer, 
                                                                  mask, offset);

}
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

    /* Moving the position of the on bits to the current mask position */
    mask = mask << ((round - 1) * num_of_bits);

    return mask; 
}

/*************************** Histogram From Int Arr ***************************/

static void HistogramFromIntArrRadix( unsigned int *histogram,
                                      unsigned int *begin_of_arr, 
                                      unsigned int *end_of_arr, 
                                      unsigned int mask, 
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

static void DeterminePositionRangeRadix(unsigned int *histogram_begin,
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

static void FillSortedArrPositionRadix(unsigned int *histogram, 
                                       unsigned int *unsorted_arr, 
                                       unsigned int *end_arr, 
                                       unsigned int *sorted_arr, 
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

/**************************** Initialize Histogram ****************************/

static void SetHistogramToZero(unsigned int *histogram, 
                               unsigned int histogram_size)
{
    unsigned int i = 0; 

    for(i = 0; i < histogram_size; i++)
    {
        histogram[i] = 0;
    }
}

/*******************************************************************************
************************* Merge Sort - comparison sort ************************/
int MergeSort(int *arr_to_sort, size_t arr_size)
{
    int *tmp_array = NULL;

    assert(NULL != arr_to_sort);

    tmp_array = MALLOC(arr_size, int);

    if(NULL == tmp_array)
    {
        return FAILURE;
    }

    MergeSortHelper(arr_to_sort, arr_size, tmp_array);

    FREE(tmp_array);

    return SUCSSES;
}


static void MergeSortHelper(int *arr_to_sort, size_t arr_size, int* tmp_array)
{
    int *right_side_arr = NULL;
    int *left_side_arr  = NULL;

    int right_size = ((int)arr_size) / 2;
    int left_size  = ((int)arr_size) / 2 + ((int)arr_size) % 2;
    
    assert(NULL != arr_to_sort);
    assert(NULL != tmp_array);

    right_side_arr = arr_to_sort + right_size + ((int)arr_size) % 2;
    left_side_arr  = arr_to_sort;

    if(2 > arr_size)
    {
        return;
    }

    /* step 1: sort the left side 
    */
    MergeSort(left_side_arr, left_size);

    /* step 2: sort the right side
     */
    MergeSort(right_side_arr, right_size);

    /* step 3: merge-sort the right side and left side
     */
    MergeTwoSides(right_side_arr, right_size, left_side_arr, left_size
                                                                   , tmp_array);
}

/**************************** Merge Sort - Helper *****************************/

static void MergeTwoSides(int *right_side_arr, int right_size,
                          int *left_side_arr,  int left_size, int *tmp_array)
{
    int  total_size = right_size + left_size;

    assert(NULL != right_side_arr);
    assert(NULL != left_side_arr);

    /* marging the two side of array in the correct order into the tmp_array. 
    */
    while(right_size > 0 && left_size > 0)
    {
        if(right_side_arr[right_size - 1] > left_side_arr[left_size - 1])
        {
            tmp_array[right_size + left_size - 1] = right_side_arr[right_size - 1];
            --right_size;
        }
        else
        {
            tmp_array[right_size + left_size - 1] = left_side_arr[left_size - 1];
            --left_size;
        }
    }

    /* marging the rest of the sorted array in to the tmp array 
    */
    while(right_size > 0)
    {
        tmp_array[right_size + left_size - 1] = right_side_arr[right_size - 1];
        --right_size;
    }
    while(left_size > 0)
    {
        tmp_array[right_size + left_size - 1] = left_side_arr[left_size - 1];
        --left_size;
    }

    /* copy the tmp_array, to the permanent array. 
    */
    memcpy(left_side_arr, (void *)tmp_array, (total_size)*sizeof(int));
}

/*******************************************************************************
************************* Quick Sort - comparison sort ************************/

void QuickSort(void *base, size_t num_of_elements, size_t elem_size,
                                                           cmp_func_ty cmp_func)
{
    void *pivot_ptr = 0;

    int *right_side_arr = NULL;
    int *left_side_arr  = NULL;

    size_t right_size = 0;
    size_t left_size  = 0;

    assert(NULL != base);
    assert(NULL != cmp_func);
    assert(0 < elem_size);

    if(2 > num_of_elements)
    {
        return;
    }

    pivot_ptr = GetPivot(base, num_of_elements, elem_size, cmp_func);

    pivot_ptr = ReorderAccordingPivotValue(base, num_of_elements, elem_size, 
                                                           cmp_func, pivot_ptr);

    left_size  = (((size_t)((char *)pivot_ptr - (char *)base)) / elem_size);
    right_size = num_of_elements - (left_size +1); 

    left_side_arr  = (int *)base;
    right_side_arr = (int *)((char *)pivot_ptr + elem_size);

    QuickSort(left_side_arr,  left_size,  elem_size, cmp_func);
    QuickSort(right_side_arr, right_size, elem_size, cmp_func);

    return;
}


static void *GetPivot(void *base, size_t num_of_elements, size_t elem_size,
                                                           cmp_func_ty cmp_func)
{
    void *tmp;
    void *begin = base; 
    void *end = (void *)((char *)base + (num_of_elements -1)*elem_size);
    void *mid = (void *)((char *)base + (num_of_elements/2)*elem_size);

    if(0 < cmp_func(begin, end))
    {
        tmp = begin;
        begin = end;
        end = tmp; 
    }

    if(0 < cmp_func(begin, mid))
    {
        mid = begin; 
    }

    if(0 < cmp_func(mid, end))
    {
        mid = end; 
    }

    return mid;
}

static void *ReorderAccordingPivotValue(void *base, size_t num_of_elements, 
                                        size_t elem_size, cmp_func_ty cmp_func, 
                                         void *pivot_ptr)
{
    char *runner_right = (char *)base + (num_of_elements - 1)*elem_size;
    char *runner_left  = (char *)base;
    
    SwapData(pivot_ptr, (void *)runner_left, elem_size);
    runner_left += elem_size;

    while(runner_left <= runner_right)
    {
        while(0 < cmp_func(base, (void *)runner_left) && runner_left <= runner_right)
        {
            runner_left += elem_size; 
        }

        while(0 > cmp_func(base, (void *)runner_right) && (void *)runner_right >= base)
        {
            runner_right -= elem_size;
        }

        if(runner_left <= runner_right)
        {
            SwapData((void *)runner_left, (void *)runner_right, elem_size); 
            runner_left  += elem_size;
            runner_right -= elem_size; 
        }
    }

    SwapData(base, (void *)runner_right, elem_size);

    return runner_right;
}

static void SwapData(void *elem_1, void *elem_2, size_t elem_size)
{
    char tmp = 0;

    char *runner_1 = NULL;
    char *runner_2 = NULL;

    assert(NULL != elem_1);
    assert(NULL != elem_2);

    runner_1 = (char *)elem_1;
    runner_2 = (char *)elem_2;

    while(0 < elem_size)
    {
        tmp = *runner_1;

        *runner_1 = *runner_2;
        *runner_2 = tmp;

        ++runner_1;
        ++runner_2;

        --elem_size;
    }

}
