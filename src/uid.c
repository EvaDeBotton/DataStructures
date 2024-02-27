/******************************************************************************
* Project: UID                                                   *
*                                                                             *
* written by Eva                                                              *
*                                                                             *
* Reviewd by Doron                                                            *
*                                                                             *
******************************************************************************/

#include "uid.h" 
#include <unistd.h>

enum is_same 
{
	NOT_EQUAL  = 0, 
	EQUAL = 1  
};
 
 static size_t counter = 0; 

/*******************************************************************************
*********************************** Uid Create ********************************/

uid_ty UidCreate(void)
{
	uid_ty new_uid = {0}; 

	new_uid.time = time(NULL);
	new_uid.counter = counter;
	new_uid.pid = getpid();

	++counter;

	return new_uid; 
}

/*******************************************************************************
********************************** Uid Is Same ********************************/

int UidIsSame(uid_ty uid_1, uid_ty uid_2)
{
	return ((uid_1.time == uid_2.time) && 
		     (uid_1.counter == uid_2.counter) &&
	        (uid_1.pid == uid_2.pid));
}

/*******************************************************************************
******************************** Uid Get Bad Uid ******************************/

uid_ty UidGetBadUid(void)
{
	uid_ty bad_uid = {0};

	return bad_uid; 
}