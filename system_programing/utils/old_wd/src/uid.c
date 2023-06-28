#include <sys/types.h>		/*	pid_t			*/
#include <time.h>			/*	time_t, size_t	*/
 #include <unistd.h>		/*	getpid			*/
 
#include "uid.h"

typedef enum exit_status {FALSE = 0, TRUE = 1} exit_status_ty;

const ilrd_uid_ty UIDBadID = {0, 0, 0};

ilrd_uid_ty UIDCreate(void)
{
	static size_t counter; 
	ilrd_uid_ty my_UID; 
	
	my_UID.pid = getpid();
	my_UID.time = time(NULL);
	
	if(((time_t) -1) == my_UID.time) /*time() fail*/
	{
		return UIDBadID; 
	}
	
	my_UID.counter = counter; 
	
	++counter; 
	
	return my_UID; 
}

int UIDIsSame(ilrd_uid_ty uid1, ilrd_uid_ty uid2)
{
	if((uid1.pid == uid2.pid) && (uid1.time == uid2.time) &&
												(uid1.counter == uid2.counter))
	{
		return (int)TRUE; 
	}
	
	return (int)FALSE; 
}
