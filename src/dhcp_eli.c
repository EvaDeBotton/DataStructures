/******************************************************************************
* Project: DHCP                                                               *
*                                                                             *
* Wariten by Eva De-Botton                                                    *
*                                                                             *
* Reviewd by Liel                                                             *
*                                                                             *
******************************************************************************/
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */ 
#include <string.h> /* memcpy */
#include <limits.h>

#include "utils.h"
#include "dhcp.h"

#define RESERVED_IP       3

#define UC(num)   ((unsigned char)num)
#define CHANGE_STATE(bit_state) (bit_state ^ UC(1))
#define NUM_OF_UNIQ_PATHS(total_level ,current_level) \
                        (1 << (total_level - current_level))\

typedef struct trie_node trie_node_ty;

struct dhcp
{    
    trie_node_ty *root;
    size_t subnet_mask;
    size_t total_levels;
    unsigned char network_address[BYTES_IN_IPV];
    unsigned char reserved_address[RESERVED_IP][BYTES_IN_IPV];
}; 

struct trie_node
{
    trie_node_ty *children[NUM_OF_CHILDREN];
    trie_node_ty *parent;
    int is_occupied;
};

/***************************** Static for Create ******************************/

static void CreateReservesAdress(
                        unsigned char requested_ip[RESERVED_IP][BYTES_IN_IPV], 
                    unsigned char returned_address[RESERVED_IP][BYTES_IN_IPV]);

static DHCP_status_ty InitializeDHCP(dhcp_ty * new_dhcp, 
                                    unsigned char network_address[BYTES_IN_IPV],
                                    size_t subnet_mask, size_t bits_number);


static DHCP_status_ty AllocReservedAdress(dhcp_ty *dhcp);

static trie_node_ty *CreateNode();

/***************************** Static for Destroy *****************************/

static void RecursiveDHCPDestroy(trie_node_ty* node);

/**************************** Static for Allocate *****************************/

static int AllocateHelper(trie_node_ty* node, int direction);

static void UpdateParents(trie_node_ty* node);

static DHCP_status_ty CheckIFRequstedApplyNetwork(dhcp_ty *dhcp, 
                                const unsigned char requested_ip[BYTES_IN_IPV]);
                                
static void UpdateBitAndByteIndex(char *bit_position,
                                         size_t *bytes_index);

/**************************** Static for Free *********************************/

DHCP_status_ty CheckIFRequstedIsNotReserved(dhcp_ty *dhcp, 
                                    const unsigned char ip_to_free[BYTES_IN_IPV]);

DHCP_status_ty CheckIFRequstedIsFree(dhcp_ty *dhcp, 
                                    const unsigned char ip_to_free[BYTES_IN_IPV], 
                                    trie_node_ty **trie_runner);

static void MarkFreeUp(trie_node_ty *trie_runner);

/**************************** Static for Counter ******************************/

static size_t RecursiveOccupiedCounter(trie_node_ty *node, size_t total_levels, 
                                                           size_t current_level);

/***************************** Bitwise operation ******************************/

size_t GetBitState(unsigned char bit_array, char bit_position);

size_t IsBitEqual(unsigned char num_1, 
                  unsigned char num_2, char bit_position);

unsigned char SetBitOn(unsigned char bit_array, char bit_position);

unsigned char SetBit(unsigned char bit_array, char bit_position, 
                                              size_t bit_state);

/*******************************************************************************
********************************** DHCP Create ********************************/

dhcp_ty *DHCPCreate(unsigned char network_address[BYTES_IN_IPV], 
                    size_t subnet_mask)
{
    DHCP_status_ty status = SUCCESS;

    dhcp_ty *new_dhcp = NULL;

    size_t bits_number = BYTES_IN_IPV * BITS_IN_BYTE;

    /* check if the number of bits in the subnet mask is valid */

    assert(subnet_mask < bits_number);
    assert(NULL != network_address);

    /* allocate the DHCP maintenance struct */

    new_dhcp = MALLOC(1, dhcp_ty);

    if(NULL == new_dhcp)
    {
        return NULL; 
    } 

    /* initialize the DHCP maintenance struct */

    status = InitializeDHCP(new_dhcp, network_address, subnet_mask, bits_number);

    if(FAILURE == status)
    {
        FREE(new_dhcp);

        return NULL; 
    }

     /* create the three reserved IP */

    status = AllocReservedAdress(new_dhcp);

    if(FAILURE == status)
    {
        FREE(new_dhcp);
        FREE(new_dhcp->root);

        return NULL; 
    }

    return new_dhcp;
}

/***************************** Static for Create ******************************/

static DHCP_status_ty InitializeDHCP(dhcp_ty *new_dhcp, 
                                    unsigned char network_address[BYTES_IN_IPV],
                                    size_t subnet_mask, size_t bits_number)
{
    assert(NULL != new_dhcp);
    assert(NULL != network_address);

    new_dhcp->root = CreateNode();

    if(NULL == new_dhcp->root)
    {
        FREE(new_dhcp);
        return FAILURE; 
    }

    new_dhcp->total_levels = bits_number - subnet_mask;
    new_dhcp->subnet_mask  = subnet_mask;

    memcpy(new_dhcp->network_address, network_address, BYTES_IN_IPV);

    return SUCCESS;
}

/******************************************************************************/

static trie_node_ty *CreateNode()
{
    trie_node_ty *node = NULL;

    node = MALLOC(1, trie_node_ty);

    if(NULL == node)
    {
        return NULL; 
    } 

    node->children[LEFT] = NULL;
    node->children[RIGHT] = NULL;
    node->is_occupied = ILRD_FREE;
    node->parent = NULL;

    return node;
}

/******************************************************************************/

static DHCP_status_ty AllocReservedAdress(dhcp_ty *dhcp)
{
    DHCP_status_ty status = SUCCESS;

    unsigned char requested_ip[RESERVED_IP][BYTES_IN_IPV];
    unsigned char returned_address[RESERVED_IP][BYTES_IN_IPV];

    size_t i = 0;

    assert(NULL != dhcp);

    CreateReservesAdress(requested_ip, returned_address);
      
    for(i = 0; i < RESERVED_IP; ++i)
    {
        status = DHCPAllocateIp(dhcp, requested_ip[i], returned_address[i]);
        
        if(FAILURE == status)
        {
            return status;
        }
    } 

    memcpy(dhcp->reserved_address, returned_address, BYTES_IN_IPV);                                               
    
    return status; 
}

/******************************************************************************/

static void CreateReservesAdress(
                        unsigned char requested_ip[RESERVED_IP][BYTES_IN_IPV], 
                    unsigned char returned_address[RESERVED_IP][BYTES_IN_IPV])
{
    int i = 0;

    assert(NULL != requested_ip);
    assert(NULL != returned_address); 

    for(i = 0; i < BYTES_IN_IPV; ++i)
    {
        requested_ip[0][i] = 0;
        requested_ip[1][i] = 0;
        requested_ip[2][i] = 255;

        returned_address[0][i] = 0;
        returned_address[1][i] = 0;
        returned_address[2][i] = 0;
    } 

    requested_ip[1][BYTES_IN_IPV - 1] = 1;
}

/*******************************************************************************
********************************** DHCP Destroy *******************************/

void DHCPDestroy(dhcp_ty *dhcp)
{
    if(NULL == dhcp)
    {
        return;
    }
    
    RecursiveDHCPDestroy(dhcp->root);
    
    FREE(dhcp);
}

/***************************** Static for Destroy *****************************/

static void RecursiveDHCPDestroy(trie_node_ty* node)
{
    if(NULL == node)
    {
        return;
    }

    RecursiveDHCPDestroy(node->children[LEFT]);  
    RecursiveDHCPDestroy(node->children[RIGHT]);
    
    FREE(node);
}

/*******************************************************************************
******************************* DHCP Allocate Ip ******************************/

DHCP_status_ty DHCPAllocateIp(  dhcp_ty *dhcp, 
                                const unsigned char requested_ip[BYTES_IN_IPV], 
                                  unsigned char returned_address[BYTES_IN_IPV])
{
	int status = SUCCESS;
	 
	size_t level_counter  = 0;
    size_t bits_in_chanks = 0;
    size_t max_tree_level = 0;
    size_t bytes_index    = 0;

    int direction = 0;
 
   	trie_node_ty* runner = dhcp->root;
    
    assert(NULL != dhcp);
    
    memcpy(returned_address, dhcp->network_address, BYTES_IN_IPV);
    
    max_tree_level = (CHAR_BIT * BYTES_IN_IPV - dhcp->subnet_mask);
    bytes_index    = (dhcp->subnet_mask) / BITS_IN_BYTE;
    bits_in_chanks = BITS_IN_BYTE - (dhcp->subnet_mask) % BITS_IN_BYTE - 1UL;

    for(level_counter = 0 ; level_counter < max_tree_level; ++level_counter)
    {
        direction = ((requested_ip[bytes_index] & (1UL << bits_in_chanks)) >> bits_in_chanks);
        
        /* need to create new node to travel the tree */
        if(NULL == runner->children[direction])
        {
            if(FAILURE ==AllocateHelper(runner, direction))
            {
                return FAILURE;
            }
        }
        /* node already exist, go there */
        else
        {
        	/* address is occupied, go somewhere else */
        	if(ILRD_OCCUPIED == runner->is_occupied)
        	{
                if(FAILURE == AllocateHelper(runner, !direction))
                {
                    return FAILURE;
                }
                
               direction = !direction;
               status = DIFFERENT_ADDRESS;
            }
         }

         returned_address[bytes_index] = SetBit(returned_address[bytes_index],
         				                       bits_in_chanks, direction);
         runner = runner->children[direction];
    
         ++level_counter;
         --bits_in_chanks;

         if(0 == bits_in_chanks)
         {
           	bits_in_chanks = (1UL << (CHAR_BIT-1));
            ++bytes_index;
         }
         else
         {
           	bits_in_chanks = bits_in_chanks >> 1UL;
         } 
    }
    
    /* leaf -> mark as occupied and update tree */
    runner->is_occupied = ILRD_OCCUPIED;
    UpdateParents(runner->parent);

    return status;
}

/**************************** Static for Allocate *****************************/

static int AllocateHelper(trie_node_ty* node, int direction)
{
    node->children[direction] = CreateNode();
    if(NULL == node)
    {
        return -1;
    }
    node->children[direction]->parent = node;
    node = node->children[direction];

    return SUCCESS;
} 

static void UpdateParents(trie_node_ty* node)
{
    while (node->parent)
    {
        if(NULL == node->children[LEFT] || 
           NULL == node->children[RIGHT] ||
           (ILRD_FREE == node->children[LEFT]->is_occupied) ||
           (ILRD_FREE == node->children[RIGHT]->is_occupied))
        {
            return;
        }

        node->is_occupied = ILRD_OCCUPIED;
        node = node->parent;
    }
}

static DHCP_status_ty CheckIFRequstedApplyNetwork(dhcp_ty *dhcp, 
                                const unsigned char requested_ip[BYTES_IN_IPV])
{
    char bit_position = BITS_IN_BYTE - 1;
    size_t bytes_index = 0;
    size_t i = 0;

    assert(NULL != requested_ip);
    assert(NULL != dhcp);

    for(i = 0; i < dhcp->subnet_mask; ++i)
    {
        if(ILRD_NOT_EQUAL == IsBitEqual(requested_ip[bytes_index], 
                             dhcp->network_address[bytes_index], bit_position))
        {
            return INVALID_ADDRESS;
        }

        UpdateBitAndByteIndex(&bit_position, &bytes_index);
    }

    return SUCCESS;
}


static void UpdateBitAndByteIndex(char *bit_position,
                                         size_t *bytes_index)
{
    assert(NULL != bit_position);
    assert(NULL != bytes_index);

    --(*bit_position);

    if(0 == (*bit_position + 1) % BITS_IN_BYTE)
    {
        *bit_position = BITS_IN_BYTE - 1;
        ++(*bytes_index);
    } 
}

/******************************************************************************/

static void MarkOccupyUp(trie_node_ty *trie_runner)
{
    assert(NULL != trie_runner);

    trie_runner->is_occupied = ILRD_OCCUPIED;

    while(ILRD_OCCUPIED == trie_runner->is_occupied)
    {
        trie_runner = trie_runner->parent;

        if( NULL != trie_runner->children[LEFT] && 
            NULL != trie_runner->children[RIGHT])
        {
            if( ILRD_OCCUPIED == trie_runner->children[LEFT]->is_occupied && 
                ILRD_OCCUPIED == trie_runner->children[RIGHT]->is_occupied) 
            {
                trie_runner->is_occupied = ILRD_OCCUPIED;
            }
        }
    }
}

/*******************************************************************************
********************************* DHCP Free Ip ********************************/

DHCP_status_ty DHCPFreeIp(dhcp_ty *dhcp, 
                          const unsigned char ip_to_free[BYTES_IN_IPV])
{
    DHCP_status_ty status = SUCCESS; 
    
    trie_node_ty *trie_runner = NULL;

    assert(NULL != ip_to_free);
    assert(NULL != dhcp);

    status = CheckIFRequstedApplyNetwork(dhcp, ip_to_free);

    if(INVALID_ADDRESS == status)
    {
        return INVALID_ADDRESS;
    }
    
    status = CheckIFRequstedIsNotReserved(dhcp, ip_to_free);

    if(UNAUTHORIZED_ADDRESS == status)
    {
        return UNAUTHORIZED_ADDRESS;
    }

    status = CheckIFRequstedIsFree(dhcp, ip_to_free, &trie_runner);

    if(DOUBLE_FREE == status)
    {
        return DOUBLE_FREE;
    }

    MarkFreeUp(trie_runner);

    return SUCCESS;
}

/******************************************************************************/

DHCP_status_ty CheckIFRequstedIsNotReserved(dhcp_ty *dhcp, 
                                const unsigned char ip_to_free[BYTES_IN_IPV])
{
    int i = 0;

    assert(NULL != dhcp);
    assert(NULL != ip_to_free);

    for(i = 0; i < RESERVED_IP; i++)
    {
        if(ILRD_EQUAL == memcmp((const char *)dhcp->reserved_address[i], 
                                 (const char *)ip_to_free, BYTES_IN_IPV))
        {
            return UNAUTHORIZED_ADDRESS;
        }
    }

    return SUCCESS;
}

/******************************************************************************/

DHCP_status_ty CheckIFRequstedIsFree(dhcp_ty *dhcp, 
                                    const unsigned char ip_to_free[BYTES_IN_IPV], 
                                    trie_node_ty **trie_runner)
{
    char bit_position = 0;
    size_t bytes_index  = 0;
    size_t i = 0;

    assert(NULL != trie_runner);
    assert(NULL != ip_to_free);
    assert(NULL != dhcp);

    *trie_runner = dhcp->root;

    bytes_index  = (dhcp->subnet_mask) / BITS_IN_BYTE;
    bit_position = BITS_IN_BYTE - (dhcp->subnet_mask) % BITS_IN_BYTE - 1;

    for(i = 0 ; i < dhcp->total_levels; ++i)
    {
        unsigned char bit_state = GetBitState(ip_to_free[bytes_index] ,bit_position);

        if(NULL != (*trie_runner)->children[bit_state])
        {
            *trie_runner = (*trie_runner)->children[bit_state];
        }
        else 
        {
            return DOUBLE_FREE;
        }

        --bit_position;
        
        if(0 == (bit_position + 1) % BITS_IN_BYTE)
        {
            bit_position = BITS_IN_BYTE - 1;

            ++bytes_index;
        } 
    }

    if(ILRD_FREE == (*trie_runner)->is_occupied)
    {
        return DOUBLE_FREE;
    }

    return SUCCESS;
}

/******************************************************************************/

static void MarkFreeUp(trie_node_ty *trie_runner)
{
    assert(NULL != trie_runner);

    trie_runner->is_occupied = ILRD_FREE;

    while(NULL != trie_runner->parent && 
                        ILRD_FREE  != trie_runner->parent->is_occupied)
    {
        trie_runner = trie_runner->parent;

        trie_runner->is_occupied = ILRD_FREE;
    }
}

/*******************************************************************************
******************************* DHCP Count Free *******************************/

size_t DHCPCountFree(const dhcp_ty *dhcp)
{
    size_t number_of_free = 0;
    size_t number_of_occupied = 0;

    size_t current_level = 0; 
    size_t total_levels = 0;

    assert(NULL != dhcp);
    assert(NULL != dhcp->root);

    total_levels = dhcp->total_levels;

    number_of_occupied = RecursiveOccupiedCounter(dhcp->root, total_levels, 
                                                              current_level);

    number_of_free = NUM_OF_UNIQ_PATHS(total_levels, current_level)
                                                           - number_of_occupied;

    return number_of_free;
}

/***************************** Static for Count *******************************/

static size_t RecursiveOccupiedCounter(trie_node_ty *node, size_t total_levels, 
                                                           size_t current_level)
{
    size_t counter = 0;

    assert(NULL != node);

    if(ILRD_OCCUPIED == node->is_occupied)
    {
        return (NUM_OF_UNIQ_PATHS(total_levels , current_level));
    }

    if(current_level == total_levels)
    {
        return counter;  
    }

    if(NULL != node->children[RIGHT])
    {
        counter += RecursiveOccupiedCounter(node->children[RIGHT], total_levels,
                                            current_level + 1);
    }

    if(NULL != node->children[LEFT])
    {
        counter += RecursiveOccupiedCounter(node->children[LEFT], total_levels,
                                            current_level + 1);
    }

    return counter; 
}


/***************************** Bitwise operation ******************************/

unsigned char SetBit(unsigned char bit_array, char bit_position,
                                              size_t bit_state)                               
{   
    bit_array = SetBitOn(bit_array, bit_position);
    bit_state = bit_state ^ UC(1);
    
    return (bit_array ^ (bit_state << bit_position)); 
}

unsigned char SetBitOn(unsigned char bit_array, char bit_position)
{
    return (bit_array | (UC(1) << (bit_position)));  
}

size_t GetBitState(unsigned char bit_array, char bit_position)
{   
    return ((bit_array & (UC(1) << bit_position)) >> bit_position); 
}

size_t IsBitEqual(unsigned char array_1, 
                  unsigned char array_2, char bit_position)
{
    return (((array_1 ^ array_2) & (UC(1) << bit_position)) >> bit_position); 
}


