/******************************************************************************
* Project: Sorted List                                                        *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Liel                                                             *
*                                                                             *
******************************************************************************/
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dlist.h"
#include "sorted_list.h"



enum is_successful 
{
    ILRD_FALSE = 0, 
    ILRD_TRUE  = 1 
};


#define FREE(ptr) {free(ptr); ptr = NULL;}

struct sorted_list
{
    dlist_ty *dlist;
    cmp_func_ty criteria_func;
};

static sorted_list_iter_ty FindSortedPosition(cmp_func_ty cmp_func, 
         sorted_list_iter_ty from, sorted_list_iter_ty to, const void *to_find);

/*******************************************************************************
****************************** Sorted List Create *****************************/

sorted_list_ty *SortedListCreate(cmp_func_ty cmp_func)
{
    sorted_list_ty *sorted_list = (sorted_list_ty *)malloc(sizeof(sorted_list_ty));
    
    assert(cmp_func);

    if (NULL == sorted_list)
    {
        return NULL;
    }
    
    sorted_list->dlist = DListCreate();
    if(NULL ==  sorted_list->dlist)
    {
      FREE( sorted_list)

      return NULL;
    } 
    
    sorted_list->criteria_func = cmp_func;
    
    return sorted_list;
}

/*******************************************************************************
***************************** Sorted List Destroy *****************************/

void SortedListDestroy(sorted_list_ty *list)
{
   assert(list);
   assert(list->dlist);

   DListDestroy(list->dlist);

   FREE(list)
}

/*******************************************************************************
****************************** Sorted List Insert *****************************/

sorted_list_iter_ty SortedListInsert(sorted_list_ty *list, void *new_data)
{
    sorted_list_iter_ty inserted_list_iter = {0};

    assert(list);
    assert(list->dlist); 

    inserted_list_iter = FindSortedPosition(list->criteria_func,
                          SortedListBegin(list), SortedListEnd(list), new_data);

    inserted_list_iter.dlist_iter = DListInsert(inserted_list_iter.dlist_iter,
                                                                      new_data);

    return (inserted_list_iter);  
}

/*******************************************************************************
****************************** Sorted List Remove *****************************/

sorted_list_iter_ty SortedListRemove(sorted_list_iter_ty iter_to_remove)
{
   assert(iter_to_remove.dlist_iter);

   iter_to_remove.dlist_iter = DListRemove(iter_to_remove.dlist_iter);

   return (iter_to_remove);
}

/*******************************************************************************
***************************** Sorted List Pop Front ***************************/

void *SortedListPopFront(sorted_list_ty *list)
{
   assert(NULL != list);
   assert(NULL != list->dlist);

   return (DListPopFront(list->dlist));
}

/*******************************************************************************
****************************** Sorted List Pop Back ***************************/

void *SortedListPopBack(sorted_list_ty *list)
{
   assert(NULL != list);
   assert(NULL != list->dlist);

   return (DListPopBack(list->dlist));
}

/*******************************************************************************
****************************** Sorted List Is Empty ***************************/

int SortedListIsEmpty(const sorted_list_ty *list)
{
   assert(list);

   return DListIsEmpty(list->dlist);
}

/*******************************************************************************
******************************** Sorted List Size *****************************/

size_t SortedListSize(const sorted_list_ty *list)
{
   assert(list);
   assert(NULL != list->dlist);

   return DListSize(list->dlist);
}

/*******************************************************************************
******************************** Sorted List Find *****************************/

sorted_list_iter_ty SortedListFind(sorted_list_ty *list 
        , sorted_list_iter_ty from, sorted_list_iter_ty to, const void *to_find)
{
    assert(list);

    return (SortedListFindIf(list->criteria_func, from, to, to_find));
}

/*******************************************************************************
****************************** Sorted List Find If ****************************/

sorted_list_iter_ty SortedListFindIf(cmp_func_ty cmp_func,
          sorted_list_iter_ty from, sorted_list_iter_ty to, const void *to_find)
{
    sorted_list_iter_ty iter = FindSortedPosition(cmp_func, from, to, to_find);

    return iter;
}

/*******************************************************************************
****************************** Sorted List For Each ***************************/

int SortedListForEach(sorted_list_iter_ty from, sorted_list_iter_ty to, 
                                                  operations_ty op, void *param)
{
   assert(from.dlist_iter);
   assert(from.dlist_iter);
   assert(op);

   return (DListForEach(from.dlist_iter, to.dlist_iter, op, param));
}

/*******************************************************************************
******************************* Sorted List Merge *****************************/

void SortedListMerge(sorted_list_ty *dest_list, sorted_list_ty *src_list)
{
    sorted_list_iter_ty dest_from = {0};
    sorted_list_iter_ty dest_end = {0};
    sorted_list_iter_ty src_from = {0};
    sorted_list_iter_ty src_from_next = {0};
    sorted_list_iter_ty src_end = {0};
   
    void *dest_data   = NULL;
    void *src_data = NULL;

    assert(NULL != dest_list);
    assert(NULL != src_list);

    dest_from = SortedListBegin(dest_list);
    dest_end = SortedListEnd(dest_list);
    src_from = SortedListBegin(src_list);
    src_from_next = SortedListNext(src_from);
    src_end = SortedListEnd(src_list);
    
    while (!SortedListIsEmpty(src_list))
    {
        dest_data = DListGetData(dest_from.dlist_iter);
        src_data = DListGetData(src_from.dlist_iter);
        
        if (dest_list->criteria_func(dest_data,src_data) >= 0)
        {
            DListSplice(src_from.dlist_iter, src_from_next.dlist_iter,
                                          SortedListPrev(dest_from).dlist_iter);

            src_from = SortedListBegin(src_list);
            src_from_next = SortedListNext(src_from);
        }
        else
        {
            dest_from = SortedListNext(dest_from);
        }
        
        if (SortedListIsEqual(dest_from,dest_end))
        {
            DListSplice(src_from.dlist_iter,src_end.dlist_iter
                                         ,SortedListPrev(dest_from).dlist_iter);
        }
    }
    
}

/******************************************************************************/

static sorted_list_iter_ty FindSortedPosition(cmp_func_ty cmp_func, 
          sorted_list_iter_ty from, sorted_list_iter_ty to, const void *to_find)
{
    assert(NULL != cmp_func);
    assert(NULL != to_find);

    while(ILRD_FALSE == SortedListIsEqual(from, to))
    {
        if(0 <= cmp_func(SortedListGetData(from), to_find))
        {
            return from;
        }

        from = SortedListNext(from);
    }

    return to;
}


/***************************** iterator functions *****************************/

/*******************************************************************************
******************************* Sorted List Begin *****************************/

sorted_list_iter_ty SortedListBegin(const sorted_list_ty *list)
{
    sorted_list_iter_ty iter = {0};

    assert(NULL != list);
    assert(NULL != list->dlist);

    iter.dlist_iter = DListBegin(list->dlist);

    return iter;
}

/*******************************************************************************
******************************** Sorted List End ******************************/

sorted_list_iter_ty SortedListEnd(const sorted_list_ty *list)
{
    sorted_list_iter_ty iter = {0}; 

    assert(NULL != list);
    assert(NULL != list->dlist);

    iter.dlist_iter = DListEnd(list->dlist);

    return iter;
}

/*******************************************************************************
******************************* Sorted List Next ******************************/

sorted_list_iter_ty SortedListNext(const sorted_list_iter_ty iter)
{
   sorted_list_iter_ty next_sorted_list_iter = {0};

   assert(NULL != iter.dlist_iter);
   assert(NULL != DListNext(iter.dlist_iter));

   next_sorted_list_iter.dlist_iter = DListNext(iter.dlist_iter);

   return (next_sorted_list_iter);
}

/*******************************************************************************
******************************* Sorted List Prev ******************************/

sorted_list_iter_ty SortedListPrev(const sorted_list_iter_ty iter)
{
   sorted_list_iter_ty prev_sorted_list_iter = {0};

   assert(NULL != iter.dlist_iter);
   assert(NULL != DListPrev(iter.dlist_iter));

   prev_sorted_list_iter.dlist_iter = DListPrev(iter.dlist_iter);

   return (prev_sorted_list_iter);
}

/*******************************************************************************
***************************** Sorted List Get Data ****************************/

void *SortedListGetData(sorted_list_iter_ty iter)
{
   assert(NULL != iter.dlist_iter);

   return DListGetData(iter.dlist_iter);
}

/*******************************************************************************
***************************** Sorted List Is Equal ****************************/

int SortedListIsEqual(const sorted_list_iter_ty iter1
                                              , const sorted_list_iter_ty iter2)
{
   assert(NULL != iter1.dlist_iter);
   assert(NULL != iter2.dlist_iter);
   
   return (iter1.dlist_iter == iter2.dlist_iter); 
}

