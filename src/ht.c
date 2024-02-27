/******************************************************************************
* Project: Hash Table                                                         *
*                                                                             *
* Wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by Doron Ben-David                                                  *
*                                                                             *
******************************************************************************/
#include<stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "utils.h"
#include "ht.h"
#include "dlist.h"

typedef struct hash_table
{
   size_t capacity;
   get_key_ty get_key_func;
   hash_func_ty hash_func;
   is_match_func_ty match_func;
   void *ht_arr[1];
};

static void *InitializeDlistsArray(void **arr, size_t capacity);
static void  FreeArrayOfDList(void **begin_of_arr, size_t capacity);

/*******************************************************************************
******************************* Hash Table Create *****************************/

ht_ty *HTCreate(size_t capacity, hash_func_ty hash_func, 
                           get_key_ty get_key_func, is_match_func_ty match_func)
{
   ht_ty *new_hash_table = NULL; 

   assert(hash_func);
   assert(get_key_func);
   assert(match_func);

   new_hash_table = (ht_ty *)malloc(sizeof(ht_ty) + sizeof(void *)*capacity);

   if(NULL == new_hash_table)
   {
      return NULL; 
   }

   InitializeDlistsArray(new_hash_table->ht_arr, capacity);

   if(NULL == new_hash_table->ht_arr)
   {
      FREE(new_hash_table);
      return NULL; 
   }

   new_hash_table->capacity = capacity;
   new_hash_table->get_key_func = get_key_func; 
   new_hash_table->hash_func = hash_func; 
   new_hash_table->match_func = match_func;

   return new_hash_table; 
}

static void *InitializeDlistsArray(void **arr, size_t capacity)
{ 
   size_t index = 0; 

   for(index = 0; index < capacity; i++)
   {
      arr[index] = DListCreate();

      if(NULL == arr[index])
      {
         FreeArrayOfDList(arr, index);

         return NULL; 
      }
   }
   return arr; 
}

/*******************************************************************************
******************************* Hash Table Destroy ****************************/

void HTDestroy(ht_ty* hash_table)
{
   assert(NULL != hash_table);

   FreeArrayOfDList(hash_table->ht_arr, hash_table->capacity);

   FREE(hash_table)  
}

/*******************************************************************************
******************************* Hash Table Insert *****************************/

int HTInsert(ht_ty* hash_table ,void* data)
{
   void *key = NULL;
   dlist_ty *dlist = NULL;

   size_t index = 0;

   assert(hash_table);
   assert(data);
   
   key   = hash_table->get_key_func(data);
   index = hash_table->hash_func(key);
   dlist = hash_table->ht_arr[index];

   if(DListEnd(dlist) == DListPushBack(dlist, data))
   {
      return ILRD_FAILURE;
   } 

   return ILRD_SUCSSES;
}

/*******************************************************************************
******************************* Hash Table Remove *****************************/

void HTRemove(ht_ty* hash_table ,void* key)
{
   dlist_ty *dlist = NULL;
   dlist_node_ty *node = NULL;

   size_t index = 0;

   assert(hash_table);
   assert(key);

   index = hash_table->hash_func(key);
   dlist = hash_table->ht_arr[index];

   node = DListFindInALL(dlist, hash_table->match_func, key);

   if(DListEnd(dlist)!= node)
   {
      DListRemove(node);
   }  
}

/*******************************************************************************
******************************* Hash Table Find *******************************/

void *HTFind(const ht_ty* hash_table, void* key)
{
   dlist_ty *dlist = NULL;
   dlist_iter_ty iter_found = NULL;

   size_t index = 0;
   
   assert(hash_table);
   assert(key);

   index = hash_table->hash_func(key);
   dlist = hash_table->ht_arr[index];

   iter_found = DListFindInALL(dlist, hash_table->match_func, key)ף

   if(DListEnd(dlist)!= iter_found)
   {
      return (DListGetData(iter_found));
   } 

   return NULL;
}

/*******************************************************************************
***************************** Hash Table For Each *****************************/

int HTForEach(ht_ty *hash_table, operation_ty op, void *param)
{
   int status = ILRD_SUCSSES; 

   dlist_ty *dlist = NULL;

   size_t index = 0;

   assert(op);
   assert(hash_table);

   for(index = 0; index < hash_table->capacity; index++)
   {
         dlist = hash_table->ht_arr[index];
         status = DListForEachInALL(dlist, op, param);

         if(ILRD_SUCSSES != status)
         {
            return status;
         }
   }

   return status;
}

/*******************************************************************************
***************************** Hash Table Is Empty *****************************/

int HTIsEmpty(const ht_ty *hash_table)
{
    size_t status = EMPTY;
    size_t index = 0; 

    assert(NULL != hash_table);

    for(index = 0; index < hash_table->capacity; index++)
    {
      if (NOT_EMPTY == DListIsEmpty(ht->ht_arr[index]))
      {
         return NOT_EMPTY;
      }
    }

    return EMPTY;
}

/*******************************************************************************
****************************** Hash Table Size ********************************/

size_t HTSize(const ht_ty *hash_table)
{
    size_t  = 0;
    size_t counter = 0;

    assert(NULL != hash_table);

    for (index = 0; index < hash_table->capacity; ++index)
    {
        counter += DListSize(hash_table->ht_arr[i]);
    }

    return counter;
}

/*******************************************************************************
************************* Hash Table Get Load Factor **************************/

double HTGetLoadFactor(const ht_ty *hash_table)
{   
   assert(hash_table);

   return(HTSize(hash_table)/hash_table->capacity);
}

/*******************************************************************************
***************************** Hash Table Get SD *******************************/

double HTGetSD(const ht_ty *hash_table)
{
   return 0;
}

/*******************************************************************************
************************** Free Array Of Allocation ***************************/

static void FreeArrayOfDList(void **arr, size_t capacity)
{
   void *end_of_arr   = &arr[capacity];

   while(arr != end_of_arr)
   {
      DListDestroy((dlist_ty *)*arr);
      ++arr;
   } 
}

