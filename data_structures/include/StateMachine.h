#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

/*
*	return true if str starts with '0' and end with '0' and have at 
*	least two chars. otherwize return false.  
*	note: in case of not '0' or '1' characters the function starts from the next 
*	char and examine it. 
*	complexity: time: O(n) space: O(1)
*/ 
int IsWordAccepted(const char *str); 

#endif	/*__STATE_MACHINE_H__*/
