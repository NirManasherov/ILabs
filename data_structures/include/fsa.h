/*******************************************************************************
Project:    Fixed Size Allocator
Author:     HRD28
Version:    1.0 - 16/01/2023
            1.1 - 16/01/2023:
                -   alignment to word-size
                -   undefined behaviour if "FSA_pool_size" is not acquired by
                    FSASuggestSize()
            1.2 -   fixed FSA_pool_ty typedef
            1.3 -   fixed FSA_pool_ty typedef, defined FSAInit return value
            1.4 -   FSASuggestSize() can handle unaligned "block_size"
            1.5 -   fixed comment on undefined behaviour for FSAInit
*******************************************************************************/
#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h>     /*  size_t  */

typedef struct fsa FSA_pool_ty;

/*******************************************************************************
 * returns the amount of bytes that must be allocated for the FSA pool to 
 * support "num_of_blocks" of "block_size"
 * blocks will be aligned with word-size - unalign-able "block_size" will result 
 * in a larger, align-able, required size
 * note: undefined behaviour if "num_of_blocks" is 0 or if "block_size" is
 *       smaller than word-size
 * time complexity: O(1)
*******************************************************************************/
size_t FSASuggestSize(size_t num_of_blocks, size_t block_size);

/*******************************************************************************
 * initializes the FSA_pool and returns a pointer to it
 * note: undefined behaviour if "mem_pool" is NULL, if "FSA_pool_size" is not 
 *       the value given by FSASuggestSize() or if "block_size" is smaller than
 *       word-size
 * time complexity: O(n)
*******************************************************************************/
FSA_pool_ty *FSAInit(void *mem_pool, size_t FSA_pool_size, size_t block_size);

/*******************************************************************************
 * returns a pointer to an available block, or NULL if all blocks of "FSA_pool"
 * are allocated
 * note: undefined behaviour if "FSA_pool" is NULL
 * time complexity: O(1)
*******************************************************************************/
void *FSAAlloc(FSA_pool_ty *FSA_pool);

/*******************************************************************************
 * frees the block pointed to by "block_to_free" and returns it to "FSA_pool"
 * note: undefined behaviour if "FSA_pool" or "block_to_free" are NULL
 * time complexity: O(1)
*******************************************************************************/
void FSAFree(FSA_pool_ty *FSA_pool, void *block_to_free);

/*******************************************************************************
 * returns the amount of free blocks in "FSA_pool"
 * note: undefined behaviour if "FSA_pool" is NULL
 * time complexity: O(n)
*******************************************************************************/
size_t FSACountFree(FSA_pool_ty *FSA_pool);

#endif  /*  __FSA_H__   */