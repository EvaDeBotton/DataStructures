/******************************************************************************
* Knight Tour                                                                 *
*                                                                             *
* wariten by Eva                                                              *
*                                                                             *
* Reviewd Liel                                                                *
*                                                                             *
******************************************************************************/ 
#include <stdio.h> /* size_t */
#include <assert.h> /* assert */


#include "knight_tour.h"
#include "utils.h"

/* test functions */
void TestKnightTour(void);

/*******************************************************************************
************************************ The Main  ********************************/

int main ()
{  

	/*BeginingOfTests(tests_name);*/

	/* The main test functions */ 
	TestKnightTour();
/*
	Remainder();

	EndOfTests();
	*/
	return 0; 
}

/*******************************************************************************
*********************** Test Initialize Priority Queue ***********************/

void TestKnightTour()
{ 

	pos_ty start_pos = 17; 

	time_t time_cap = 60 * 60;

	pos_ty route_dest[DIM*DIM] = {0};

	KnightsTour(start_pos, route_dest, time_cap);

}
