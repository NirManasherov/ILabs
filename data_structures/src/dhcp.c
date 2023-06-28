/*******************************************************************************
* Author: Nir Manasherov
* Project: hash table
* Reviewer: 
* Date: 9/02/2023
* Version: 1.0
*******************************************************************************/
/*******************************************************************************
                                  libraries
*******************************************************************************/

#include <stddef.h>         /*		size_t      			*/
#include <assert.h>			/*		assert		 			*/
#include <stdlib.h>			/*		malloc, calloc, free	*/
#include <assert.h>			/*		assert					*/
#include <string.h>			/*		memmove					*/

#include "dhcp.h"

/*******************************************************************************
                                  macros
*******************************************************************************/      

#define BITS_IN_IP 32

#define NUM_SAVED_ADDRESSES 3

#define NUM_OF_OPTIONS 256

#define UNUSED(x) (void)(x)

/*******************************************************************************
                                  enums
*******************************************************************************/

enum {END_IP = 0, NOT_END_IP = 1};
 
enum {FALSE = 0, TRUE = 1}; 

enum children {LEFT = 0, RIGHT = 1, NUM_OF_CHILDREN}; 

/*******************************************************************************
                                  structs
*******************************************************************************/    

typedef struct dhcp_node trie_node_ty;

struct dhcp_node
{
    trie_node_ty *parent;
    trie_node_ty *children[NUM_OF_CHILDREN];
    int is_occupied;
};

struct dhcp
{
    trie_node_ty *root;
    const ip_ty subnet_ip;
    size_t num_of_bits_in_subnet;
};

/*******************************************************************************
                                  function types
*******************************************************************************/ 

/*  write a function with the following signature to perform whichever action
    you need on "ds_data"
    "param" is extra data                                              
    return 0 if succeeded, otherwise - not 0                                  */
typedef int (*dhcp_action_func_ty)(void *dhcp_data, void *param);

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

/*
creates a new node that is set child of "parent" node
sets it's children to NULL (leaf node)
sets "is_occupied" member to specifications of "is_occupied" by macro
in case of fail return NULL
*/
static trie_node_ty *CreateNode(trie_node_ty *parent, int is_occupied); 

/*
return the bit in place "place" from the most significant bit 
rely on that the pointer ip_to_scan points on the most significant bit of ip
*/
static int GetBit(const ip_ty ip_to_scan, size_t place_from_msb);

/*
init the dhcp handle
*/
static void InitDhcp(dhcp_ty *dhcp, const ip_ty subnet_ip, 
												  size_t num_of_bits_in_subnet);

/*
add subnet ip number to the most significant bit side of the number
update the host value
*/
static void AddSubnetToHost(ip_ty subnet_ip, size_t subnet_bits_num,
															unsigned int *host);
															
/*
traverse the tree in post order and do action on each node once
*/
static size_t PostOrderTraverse(dhcp_ty *dhcp, dhcp_action_func_ty act_func,
																   void *param);

/*
destroy one node
*/
static int DestroyNode(void *node, void *params); 

/*
add to counter got the "is occupied" value
*/
static int CountOccupied(void *dhcp_data, void *param);

/*
allocate IP in TRIE tree
check for if equal to the saved IPs only if flag is_check_save is TRUE
*/
static status_ty DHCPAllocateIPChooseCheck(dhcp_ty *dhcp,
              const ip_ty requested_ip, ip_ty allocated_ip, int is_check_saved);

/*******************************************************************************
                        Static functions declerations
*******************************************************************************/

static unsigned int saved_host_ip_adresses[NUM_SAVED_ADDRESSES] = {0, 1, 255};

/*******************************************************************************
                        Functions implimentations
*******************************************************************************/

dhcp_ty *DHCPCreate(const ip_ty subnet_ip, size_t subnet_bits_num)
{
	
	dhcp_ty *dhcp = NULL;
	size_t i = 0; 
	
	/*we need at least2 nodes to define the saved adresses*/
	assert(subnet_bits_num < 31);
	assert(NULL != subnet_ip);
	
	/*allocate resources for dhcp handler*/
	dhcp = (dhcp_ty *)malloc(sizeof(dhcp_ty)); 
	
	/*malloc can fail*/
	if(NULL == dhcp)
	{
		return NULL;
	}
	
	/*init dhcp hadler*/
	InitDhcp(dhcp, subnet_ip, subnet_bits_num);
	
	/*set subnet to saved host numbers and set it to tree*/
	for(i = 0 ; i < sizeof(saved_host_ip_adresses) ; ++i)
	{
		AddSubnetToHost((unsigned char *)subnet_ip, subnet_bits_num, 
													saved_host_ip_adresses + i);
	
		/*allocate saved ip addresses*/
		DHCPAllocateIPChooseCheck(dhcp, 
							(const unsigned char *)(saved_host_ip_adresses + i), 
					   	  (unsigned char *)(saved_host_ip_adresses + i), FALSE);
	}
	
	/*return handle to dhcp*/
	return dhcp; 
}

/******************************************************************************/

/*
init the dhcp handle
*/
static void InitDhcp(dhcp_ty *dhcp, const ip_ty subnet_ip, 
												   size_t num_of_bits_in_subnet)
{
	dhcp->root = CreateNode(NULL, (int)NOT_END_IP);
	memmove((void *)(dhcp->subnet_ip) ,(const void *)subnet_ip, IP_OCTET_NUM); 
	dhcp->num_of_bits_in_subnet = num_of_bits_in_subnet; 
}

/******************************************************************************/

void DHCPDestroy(dhcp_ty *dhcp)
{
	assert(NULL != dhcp);
	
	PostOrderTraverse(dhcp, DestroyNode, NULL); 
	
	free(dhcp);
	dhcp = NULL;  
	
}

/******************************************************************************/

/*
destroy one node
*/
static int DestroyNode(void *node, void *params)
{	
	UNUSED(params); 
	
	free(node); 
	node = NULL; 
	
	return SUCCESS; 
}

/******************************************************************************/

/*
traverse the tree in post order and do action on each node once
*/
static size_t PostOrderTraverse(dhcp_ty *dhcp, dhcp_action_func_ty act_func,
																    void *param)
{
	size_t counter = 0;
    trie_node_ty *curr = NULL;
	trie_node_ty *prev = NULL;

    assert(NULL != dhcp);

    curr = dhcp->root;
	prev = NULL;

	while (curr->parent != NULL)
	{
		if (NULL == curr)
		{
			curr = prev;
			prev  = NULL;
		}
		else if (prev == (curr->parent))
		{
			prev = curr;
			curr = curr->children[LEFT];
		}
		else if (prev == (curr->children[RIGHT]))
		{
			prev = curr;
			curr = curr->parent;
            ++counter;
            if (act_func)
            {
			    act_func(prev, param);
            }
		}
		else
		{
            assert(prev == (curr->children[LEFT]));
			prev = curr;
			curr = curr->children[RIGHT];
		}
	}
    return counter;
}

/******************************************************************************/

status_ty DHCPAllocateIP(dhcp_ty *dhcp,
                                const ip_ty requested_ip, ip_ty allocated_ip)
{
	assert(NULL != dhcp);
	assert(NULL != allocated_ip);
	
	return DHCPAllocateIPChooseCheck(dhcp, requested_ip, allocated_ip, TRUE);
	
}

/******************************************************************************/

/*
allocate IP in TRIE tree
check for if equal to the saved IPs only if flag is_check_save is TRUE
*/
static status_ty DHCPAllocateIPChooseCheck(dhcp_ty *dhcp,
               const ip_ty requested_ip, ip_ty allocated_ip, int is_check_saved)
{
	size_t i = 0; 
	int curr_bit = -1; 
	ip_ty subnet_ip;
	size_t num_of_bits_in_subnet = 0; 
	trie_node_ty *curr_node; 
	int *requested_ip_int = 0;
	ip_ty new_requested_ip; 
	unsigned int *saved_addresses_ptr = NULL;
	unsigned int lowest_valid_host = 2;
	
	/*get subnet information*/
	num_of_bits_in_subnet = dhcp->num_of_bits_in_subnet;
	memmove(subnet_ip ,dhcp->subnet_ip, IP_OCTET_NUM); 
	
	/*not default settings*/
	if(NULL != requested_ip)
	{
		memmove(new_requested_ip, requested_ip, IP_OCTET_NUM);
	}
	
	/*check if user wants default settings*/
	/*NULL == requested_ip -> make it the lowest address possible*/
	else
	{
		AddSubnetToHost((unsigned char *)subnet_ip,num_of_bits_in_subnet, 
															&lowest_valid_host); 
		
		/*set IP to host no. = 2 which is the lowest valid host no.*/		  
		memmove((void *)new_requested_ip ,&lowest_valid_host , IP_OCTET_NUM);
	}
	
	/*get access to saved adresses list*/
	saved_addresses_ptr = saved_host_ip_adresses;
	
	/*return error if user is trying to get saved address*/
	for(i = 0 ; i < (sizeof(saved_addresses_ptr)&&(is_check_saved)) ; ++i)
	{
		/*make ip from host saved numbers*/
		AddSubnetToHost((unsigned char *)subnet_ip, num_of_bits_in_subnet, 
													saved_addresses_ptr + i);
		
		/*if the requested adress is saved return fail*/
		if(0 == memcmp(saved_addresses_ptr, new_requested_ip, IP_OCTET_NUM))
		{
			return FAILURE;
		}
		
		/*get next saved adress*/
		++saved_addresses_ptr;
	}
	
	curr_bit = GetBit(new_requested_ip, i); 
	
	/*check the subnet*/
	for(i = 0 ; i < num_of_bits_in_subnet ; ++i)
	{
		/*make sure subnet and requested ip starting is the same*/
		if(GetBit(subnet_ip , i) != GetBit(new_requested_ip , i )) 
		{
			return FAILURE;
		}
	}
	
	curr_node = dhcp->root;
	
	/*follow ip branch, allocate it if needed*/
	for(i = num_of_bits_in_subnet ; i < BITS_IN_IP ; ++i)
	{
		/*get  the current bit*/
		curr_bit = GetBit(new_requested_ip , i );
		
		/*allocates last node */
		if(i == BITS_IN_IP - 1)
		{
			curr_node->children[curr_bit] = CreateNode(curr_node, TRUE);	
			
			memmove(allocated_ip, new_requested_ip, IP_OCTET_NUM);
			
			return SUCCESS; 
		}
		
		/*if there is no child - make one*/
		if(NULL == curr_node->children[curr_bit])
		{
			curr_node->children[curr_bit] = CreateNode(curr_node, FALSE);
			curr_node = curr_node->children[curr_bit];
		}
	}
	
	requested_ip_int = (int *)new_requested_ip;
	
	++requested_ip_int;
	
	memmove(new_requested_ip, requested_ip_int, IP_OCTET_NUM);
	
	/*if occupied*/
	return DHCPAllocateIP(dhcp, new_requested_ip, allocated_ip);
}

/******************************************************************************/

status_ty DHCPFreeIP(dhcp_ty *dhcp, ip_ty ip_to_free)
{
	size_t num_of_bits_in_subnet = 0; 
	ip_ty subnet_ip;
	trie_node_ty *curr_node; 
	unsigned int *saved_addresses_ptr = NULL;
	size_t i = 0;
	int curr_bit = -1;  
	
	assert(NULL != dhcp); 
	assert(NULL != ip_to_free);
	
	num_of_bits_in_subnet = dhcp->num_of_bits_in_subnet;
	memmove(subnet_ip ,dhcp->subnet_ip, IP_OCTET_NUM); 
	
	saved_addresses_ptr = saved_host_ip_adresses;
	
	curr_bit = GetBit(ip_to_free, i);
	
	/*check the subnet*/
	for(i = 0 ; i < num_of_bits_in_subnet ; ++i)
	{
		/*make sure subnet and requested ip starting is the same*/
		if(GetBit(subnet_ip , i) != GetBit(ip_to_free , i )) 
		{
			return FAILURE;
		}
	}
	
	/*return error if user is trying to get saved address*/
	for(i = 0 ; i < sizeof(saved_addresses_ptr) ; ++i)
	{
		AddSubnetToHost((unsigned char *)subnet_ip, num_of_bits_in_subnet, 
													saved_addresses_ptr + i);
	
		if(0 == memcmp(saved_addresses_ptr, ip_to_free, IP_OCTET_NUM))
		{
			return FAILURE;
		}
		
		++saved_addresses_ptr;
	}
	
	curr_node = dhcp->root;
	
	/*check ip branch*/
	for(i = num_of_bits_in_subnet ; i < BITS_IN_IP ; ++i)
	{
		/*get  the current bit*/
		curr_bit = GetBit(ip_to_free , i );
		
		/*if there is no child - make one*/
		if(NULL == curr_node->children[curr_bit])
		{
			return DOUBLE_FREE_FAILURE;
		}
	}
	
	/*set ip as free*/
	if(TRUE == curr_node->is_occupied)
	{
		curr_node->is_occupied = FALSE;
	}
	
	/*if the ip has been set free already*/
	else
	{
		return DOUBLE_FREE_FAILURE;
	}
	
	
	return SUCCESS; 
}

/******************************************************************************/

size_t DHCPCountFree(const dhcp_ty *dhcp)
{
	size_t counter = 0; 
	
	PostOrderTraverse((dhcp_ty *)dhcp, CountOccupied, (void *)&counter);
	
	return (NUM_OF_OPTIONS - counter); 
}

/******************************************************************************/

/*
add to counter got the "is occupied" value
*/
static int CountOccupied(void *dhcp_data, void *param)
{
	size_t *counter = (size_t *)param; 
	trie_node_ty *curr_node = (trie_node_ty *)dhcp_data;
	
	*counter += (curr_node->is_occupied);
	
	return SUCCESS;
}

/*******************************************************************************
                      Static functions implimentation
*******************************************************************************/

/*
creates a new node that is set child of "parent" node
sets it's children to NULL (leaf node)
sets "is_occupied" member to specifications of "is_occupied" by macro
in case of fail return NULL
*/
static trie_node_ty *CreateNode(trie_node_ty *parent, int is_occupied)
{
	trie_node_ty *new_node = NULL; 
	
	new_node = (trie_node_ty *)malloc(sizeof(trie_node_ty));
	
	/*malloc can fail*/
	if(NULL == new_node)
	{
		return NULL; 
	}
	
	/*init to leaf with data*/
	new_node->parent = parent; 
	new_node->is_occupied = is_occupied;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	
	return new_node; 
}

/******************************************************************************/

/*
return the bit in place "place" from the most significant bit 
rely on that the pointer ip_to_scan points on the most significant bit of ip
*/
static int GetBit(const ip_ty ip_to_scan, size_t place_from_msb)
{
	unsigned int mask = 0; 
	unsigned int res = -1; 
	unsigned int place_from_lsb = 0; 
	
	/*get place from least significant bit*/
	place_from_lsb = BITS_IN_IP - place_from_msb; 
	
	/*make a mask for that bit*/
	mask = 1<<place_from_lsb;
	
	/*copy only that bit and return it as it's value (0 or 1) using & operator*/
	res = (*((unsigned int *)ip_to_scan)&mask)>>place_from_lsb; 
	
	/*return the resault (0 or 1)*/
	return res; 
}

/******************************************************************************/

/*
add subnet ip number to the most significant bit side of the number
update the host value
*/
static void AddSubnetToHost(ip_ty subnet_ip, size_t subnet_bits_num,
															 unsigned int *host)
{
	int subnet_ip_int = 0; 
	unsigned int place_from_lsb = 0; 
	
	/*get place from least significant bit*/
	place_from_lsb = BITS_IN_IP - subnet_bits_num; 
	
	/*set all bits of subnet to 0*/
	*host = ((*host)<<subnet_bits_num)>>subnet_bits_num;
	
	/*set all bits of host to 0*/
	subnet_ip_int = (*((int *)subnet_ip)>>place_from_lsb)<<place_from_lsb;
	
	*host = (*host)^subnet_ip_int; 
}

/******************************************************************************/
