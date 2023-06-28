/**********************************************************************/
/**************************libraries***********************************/

#include <stddef.h>

/***********************************************************************/
/**************************declerations*********************************/

#ifndef __WS9__H_ 
#define __WS9__H_


/***********************************************************************/
/*********************functions*declerations***************************/

void *Memcpy(void *dest, const void *src, size_t n); 
void *Memmove(void *dest, const void *src, size_t n);
void *Memset(void *s, int c, size_t n);

/***********************************************************************/

#endif /* __WS9__H_ */