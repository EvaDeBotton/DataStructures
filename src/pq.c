/******************************************************************************
* Project: Sorted List                                                        *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Eli                                                             *
*                                                                             *
******************************************************************************/
#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc  free*/
#include <assert.h> /* assert */

#include "pqueue.h"
#include "sorted_list.h"
#include "functions.h"

enum success 
{
    SUCCESS  =  0, 
    FAILURE  =  1 
};

#define EMPTY    1

#define FREE(ptr) {free(ptr); ptr = NULL;}

typedef struct match_adaptar
{
    is_match_ty fun;
    void *user_data;

} match_adaptar_ty;

struct pqueue
{
    sorted_list_ty *list;    

} pqueue;

static int MatchAdaptor(const void *data1, const void *match_adaptar);

/*******************************************************************************
********************************** PQ Create **********************************/

pqueue_ty *PQCreate(cmp_func_ty cmp_func)
{
    pqueue_ty *new_pq = NULL; 

    assert(cmp_func);

    new_pq = (pqueue_ty *)malloc(sizeof(pqueue_ty));

    if(NULL == new_pq)
    {
        return NULL;
    }

    new_pq->list = SortedListCreate(cmp_func);

    if(NULL == new_pq->list)
    {
        FREE(new_pq);

        return NULL; 
    }

    return new_pq; 
}

/*******************************************************************************
********************************** PQ Destroy *********************************/

void PQDestroy(pqueue_ty *pqueue)
{
    assert(pqueue); 

    SortedListDestroy(pqueue->list);

    FREE(pqueue);
}

/*******************************************************************************
********************************** PQ Enqueue *********************************/

int PQEnqueue(pqueue_ty *pqueue, void *data)
{
    sorted_list_iter_ty iter1 = {0};
    sorted_list_iter_ty iter2 = {0};
    
    assert(pqueue);

    iter1 = SortedListEnd(pqueue->list);

    iter2 = SortedListInsert(pqueue->list, data);

    return (SortedListIsEqual(iter1 ,iter2));
}

/*******************************************************************************
********************************** PQ Dequeue *********************************/

void *PQDequeue(pqueue_ty *pqueue)
{
    void *peek = NULL;

    assert(pqueue); 

    peek = PQPeek(pqueue);

    SortedListPopBack(pqueue->list);

    return peek; 
}

/*******************************************************************************
************************************ PQ Peek **********************************/

void *PQPeek(const pqueue_ty *pqueue)
{
    assert(pqueue);

    return SortedListGetData(SortedListPrev(SortedListEnd(pqueue->list)));
    
}

/*******************************************************************************
************************************ PQ Size **********************************/

size_t PQSize(const pqueue_ty *pqueue)
{
    assert(pqueue);

    return SortedListSize(pqueue->list);
}

/*******************************************************************************
********************************** PQ Is Empty ********************************/

int PQIsEmpty(const pqueue_ty *pqueue)
{
    assert(pqueue);
    
    return SortedListIsEmpty(pqueue->list);
}

/*******************************************************************************
*********************************** PQ Clear **********************************/

void PQClear(pqueue_ty *pqueue)
{
    assert(pqueue);
    
    while(EMPTY != PQIsEmpty(pqueue))
    {
        PQDequeue(pqueue);
    }
}

/*******************************************************************************
*********************************** PQ Erease *********************************/

void *PQErease(pqueue_ty *pqueue, is_match_ty is_match_func, void *user_data)
{
    match_adaptar_ty match_adaptar = {0};

    sorted_list_iter_ty iter = {0};
    sorted_list_iter_ty end_iter = {0};
    void* ret_val = NULL; 
    assert(pqueue);
    assert(is_match_func);

    match_adaptar.fun = is_match_func;
    match_adaptar.user_data = user_data;

    iter = SortedListFindIf(MatchAdaptor, SortedListBegin(pqueue->list)
      ,SortedListEnd(pqueue->list), &match_adaptar);

    end_iter = SortedListEnd(pqueue->list);
    if(SortedListIsEqual(iter, end_iter))
    {
        return ret_val;
    }
    
    ret_val = SortedListGetData(iter);

    SortedListRemove(iter);

    return ret_val;
}

/*******************************************************************************
********************************* Match Adaptor *******************************/

static int MatchAdaptor(const void *data1, const void *match_adaptar)
{
    const match_adaptar_ty* s_adaptar = match_adaptar;

    return ((0 == s_adaptar->fun(data1,s_adaptar->user_data)) ? -1 : 0);
}




