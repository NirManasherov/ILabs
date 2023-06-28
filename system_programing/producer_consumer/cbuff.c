#include <stddef.h>        /*  size_t,  offsetof  */
#include <sys/types.h>     /*  ssize_t            */
#include <stdlib.h>        /*  malloc, free       */
#include <assert.h>        /*  assert             */
#include <string.h>        /*  memcpy             */

#include "cbuff.h"

#define FAILURE -1

struct c_buff
{
    size_t read_idx;
    size_t size;
    size_t capacity;
    char arr[1];
};

/*******************************************************************************
*******************************************************************************/
c_buff_ty *CBuffCreate(size_t capacity)
{
    size_t req_size = offsetof(c_buff_ty, arr) + capacity;
    c_buff_ty *cbuff = (c_buff_ty *)malloc(req_size);
    
    if (NULL == cbuff)
    {
        return NULL;
    }
    
    cbuff->read_idx = 0;
    cbuff->size = 0;
    cbuff->capacity = capacity;
    
    return cbuff;
}

/*******************************************************************************
*******************************************************************************/
void CBuffDestroy(c_buff_ty *buffer)
{
    assert(NULL != buffer);
    
    free(buffer);
    buffer = NULL;
}

/*******************************************************************************
*******************************************************************************/
ssize_t CBuffWrite(c_buff_ty *buffer, const void *src, size_t num_bytes)
{
    size_t offset_write = 0;
    size_t first_write_bytes = 0;
    size_t free_bytes = 0;
    
    assert(NULL != buffer);
    assert(NULL != src);
    
    offset_write = (buffer->read_idx + buffer->size) % buffer->capacity;
    first_write_bytes = buffer->capacity - offset_write;
    
    free_bytes = CBuffFreeSpace(buffer);
    
    if (0 == free_bytes)
    {
        return FAILURE;
    }
    else if (free_bytes < num_bytes)
    {
        num_bytes = free_bytes;
    }
    
    if (num_bytes > first_write_bytes)
    {
        memcpy(buffer->arr + offset_write, src, first_write_bytes);
        memcpy(buffer->arr, (char *)src + first_write_bytes, 
                                                num_bytes - first_write_bytes);
    }
    else
    {
        memcpy(buffer->arr + offset_write, src, num_bytes);
    }
    
    buffer->size += num_bytes;
    
    return num_bytes;
}

/*******************************************************************************
*******************************************************************************/
ssize_t CBuffRead(c_buff_ty *buffer, void *dest, size_t num_bytes)
{
    size_t offset_read = 0;
    size_t first_write_bytes = 0;
    
    assert(NULL != buffer);
    assert(NULL != dest);
    
    offset_read = buffer->read_idx;
    first_write_bytes = buffer->capacity - offset_read;
    
    if (CBuffIsEmpty(buffer) || (0 == num_bytes))
    {
        return FAILURE;
    }
    
    if (num_bytes > buffer->size)
    {
        num_bytes = buffer->size;
    }
    
    if (num_bytes > first_write_bytes)
    {
        memcpy(dest, buffer->arr + buffer->read_idx, first_write_bytes);
        memcpy((char *)dest + first_write_bytes, buffer->arr, 
                                                num_bytes - first_write_bytes);
        
        buffer->read_idx = num_bytes - first_write_bytes;
    }
    else
    {
        memcpy(dest, buffer->arr + buffer->read_idx, num_bytes);
        
        buffer->read_idx += num_bytes;
    }
    
    buffer->size -= num_bytes;
    
    return num_bytes;
}

/*******************************************************************************
*******************************************************************************/
int CBuffIsEmpty(const c_buff_ty *buffer)
{
    assert(NULL != buffer);
    
    return (0 == buffer->size);
}

/*******************************************************************************
*******************************************************************************/
size_t CBuffSize(const c_buff_ty *buffer)
{
    assert(NULL != buffer);
    
    return buffer->size;
}

/*******************************************************************************
*******************************************************************************/
size_t CBuffFreeSpace(const c_buff_ty *buffer)
{
    assert(NULL != buffer);
    
    return buffer->capacity - buffer->size;
}