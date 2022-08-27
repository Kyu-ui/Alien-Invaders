/* Colin Fung */
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "debug.h"
#include "motion.h"
#include "lab4.h"

/* used to switch the sign of VX */
#define signSwap -1
/* positive and neg X bounds */
#define posXbound 39.0
#define negXbound  -39.0
/* decrement value of y */
#define yDec 3
/* Swaps the sign on VX */
void alienBounce(unsigned int *alien, double *x, double *y, double *VX, double time)
{
	/* Swaps sign */
	*VX = *VX *signSwap;

	/* y gets decremented by 3 */
	*y = *y - yDec;
	/*clips y to ground if neg */
	if (*y < 0) *y = 0;

	/* Prints incidental message of VX change */
	printVXChange(alien, x, y, VX, time);
}

/* Changes the value of VX if needed */
void negateVX(unsigned int *alien, double *x, double *y, double *VX, double time)
{
	
	/* If the alien is going out of bounds, make function return a negated VX */
	if (*x >= posXbound && *VX > 0.0) {
		/* Change VX will flip the sign on VX */
		alienBounce(alien, x, y, VX, time);
	}
	else if (*x <= negXbound && *VX < 0.0) {
		/* Change VX will flip the sign on VX */
		alienBounce(alien, x, y, VX, time);
	}
}


/* Updates the x and y values according to their corresponding velocitys */
void updateXandY(double *x, double *y, double *VX, double *VY, unsigned int *alien, double time)
{	
	/* Formulas for motion equations */

	*x = *x + *VX * deltaT;
	
	*y = *y + *VY * deltaT;
	/* Incase x hits a corner, negates the VX so it goes the other way, Will only negate the VX if needed */
	negateVX(alien, x, y, VX, time);
}


/* Moves the time by deltaT, prints corresponding  */
void moveTime(double *time, unsigned int *alien, double *x, double *y, double *VX, double *VY) 
{
	/* Time is incremented by deltaT */
	*time = *time + deltaT;
}




/* calls updatesXandY with right variables */
void updateMotion(void *alienStructPtr) 
{	
	/* dereferences aliena nd sim pointers */
	Alien *alienStruct = alienStructPtr;
	Sim *simStruct = alienStruct->simStructPtr;
	/* The & goes after -> resolves */
	/*Updates x and y values */
	updateXandY(&alienStruct->X, &alienStruct->Y, &alienStruct->VX, &alienStruct->VY, &alienStruct->alien, simStruct->elapsedTime);
}


