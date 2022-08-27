/* Colin Fung */
#include <stdio.h>
#include <stdlib.h>
#include "lab4.h"
#REMOVED FOR UPLOAD
#include "code.h"
#include "debug.h"
#include "output.h"
/* --------------------------------------------------- */
/* TABULAR OUTPUT FUNCTIONS */
/* --------------------------------------------------- */

/* Prints Tabular text output, the chart form of text output */
void tabularTextOutputHeader(void *alienStruct)
{
	/* dereference alien and sim pointers */
	Alien *alienStructPtr = alienStruct;
	Sim *simStruct = alienStructPtr->simStructPtr;
	/* Prints the first line with correct spacing */
	if(TEXT) printf("\nCode       T C Pts ( __X_____, __Y_____) ( __VX____,  __VY____) ET= %0.5lf\n", simStruct->elapsedTime);
}

/* Prints Tabular text output, the chart form of text output */
void tabularTextOutput(void *alienStruct)
{
	/* dereference alien and sim pointers */
	Alien *alienStructPtr = alienStruct;
	Sim *simStruct = alienStructPtr->simStructPtr;
	/* Prints second line with modifiers so number are aligned with header tab output */
	if(TEXT) printf("0x%0.8X %d %d %3d (%9.5lf, %8.5lf) (%9.5lf, %9.5lf)\n", alienStructPtr->alien, alienStructPtr->type, alienStructPtr->color, alienStructPtr->points, alienStructPtr->X, alienStructPtr->Y, alienStructPtr->VX, alienStructPtr->VY);
}

	

/* ------------------------------------------- */
/* Functions for Incidental Messages */
/* ------------------------------------------- */

/* Prints debug message for when the time is equivalent to a whole number so in regular input you output it */
void printForDebugCrossBoundary(int timeNow, int prevTime, double ET, int returnValue)
{
	printf("crosses_second_boundary: Cur = %d, Prev = %d, ET = %0.5lf: returning %d\n", timeNow, prevTime, ET, returnValue);
}

/* Prints the incidental message for if the aliens VX changed due to hitting a wall (x boundary) */
void printVXChange(unsigned int *alien, double *x, double *y, double *VX, double time) 
{
		printf("\nAlien 0x%0.8X drops to (%9.5lf, %8.5lf), VX to %lf) at ET %lf\n", *alien, *x, *y, *VX, time+deltaT);
}

/* Print message for bolt hjitting */
void printForBoltHit(void *alienStruct)
{
	/* dereference alien pointer */
	Alien *alienStructPtr = alienStruct;

	/* prints wjhehter in graphics or text the bolt hitting */
	if (TEXT) printf("Bolt hits alien at (%.5lf, %.5lf), scores %d points!\n", alienStructPtr->X, alienStructPtr->Y, alienStructPtr->points);
	if (GRAPHICS) sa_status("Bolt hits an alien!"); 
}

/* Prints message for saucer esacpaing */
void printForSaucerEscape(void *alienStruct)
{
	/* dereference sim and alien pointers */
	Alien *alienStructPtr = alienStruct;
	Sim *simStructPtr = alienStructPtr->simStructPtr;
	/* Prints message for escape whether in texct or grpahics */
	if (TEXT) printf("Flying saucer worth %d points escapes at ET = %lf\n", alienStructPtr->points, simStructPtr->elapsedTime);
	if (GRAPHICS) sa_status("Flying saucer escapes!"); 
}

/* Print touchdown message */
void printForTouchDown(void *alienStruct)
{
	/* dereference alien pointer */
	Alien *alienStructPtr = alienStruct;
	/* prints touchdown message */
	if (TEXT) printf("\n\nAlien 0x%0.8X touches down at X=%lf!\n", alienStructPtr->alien, alienStructPtr->X);
}

/* Prints defeat or victory message */
void printDefeatOrVictory(void **alien_head_list, void *simStruct, int alienTouchGround)
{

	/*dereference sim pointer */
	Sim *simStructPtr = simStruct;

	/* If alien touched ground, print defeat message, otherwise print victory message */
	if(!alienTouchGround) 
	{
		if(TEXT)printf("\nVictory! No attackers remain at ET = %lf\n", simStructPtr->elapsedTime);
	}
	else 
	{
		if(TEXT) printf("\nDefeat! Aliens land at ET = %lf\n", simStructPtr->elapsedTime);
	}
}		

/* Prints the score for text mode */
void printScore(void *simStruct)
{
	Sim *simStructPtr = simStruct;
	printf("Total score is %d points\n", simStructPtr->score);
}

/* Prints message for clearing list at the start */
void printAliensClearingStart()
{
	printf("Clearing the attacking list...\n");
}

/* Prints message for clearing list */
void printAliensCleared(int aliensCleared)
{
	printf("    ... %d cleared\n", aliensCleared);
}



/* Prints the total run time ( finish - start) of entire simulation */
void printRuntime(double startTime) 
{
	double totalTime = now();
	totalTime = totalTime - startTime;
	printf("Total run time is %0.9lf seconds.\n", totalTime);
}

/* Prints that the scan has terminated with the value it scanf returned */
void printScanTerminated(double returnValue) 
{
	printf("Input terminated: scanf returned %1.0lf\n", returnValue);
}


/* ------------------------------------------- */
/* Functions for code.c */
/* ------------------------------------------- */

/* Prints the debug message for the type of the alien gotten from bit manip */
void printFordebugType(unsigned int isolatedBits, int shiftedValue, unsigned int alien, unsigned int returnedValue)
{
	printf("type_from_code: Turning on values at (in hex) 0x%X after shifting %d on 0x%X gives %X (in hex)\n", isolatedBits, shiftedValue, alien, returnedValue);
}

/* Prints the debug message for the color of the alien gotten from bit manip */
void printForDebugColor(unsigned int isolatedBits, int shiftedValue, unsigned int alien, unsigned int returnedValue)
{
	printf("Color_from_code: Turning on values at (in hex) 0x%X after shifting %d on 0x%X gives %X (in hex)\n", isolatedBits, shiftedValue, alien, returnedValue);
}

/* Prints the debug message for the points of the alien gotten from bit manip */
void printForDebugPoints(unsigned int isolatedBits, int shiftedValue, unsigned int alien, unsigned int returnedValue)
{
	printf("Points_from_code: Turning on values at (in hex) 0x%X after shifting %d on 0x%X gives %X (in hex)\n", isolatedBits, shiftedValue, alien, returnedValue);
}

/* ----------------------------------------------
 * Drawing alien functions
 *--------------------------------------- */
 


/* Starts alien graphcics */
void startAlienGraphics(void *alienStruct)
{
	Alien *alienStructPtr = alienStruct;
	/* puts in a single alien */
	sa_alien(alienStructPtr->type, alienStructPtr->color, alienStructPtr->X, alienStructPtr->Y);
}



/* Draws aliens for each alien in list */
void drawAliens(void **alien_head_list)
{
	/* Function of running through a single alien graphic */
	ActionFunction alienStartGraphic = &startAlienGraphics;
	/* goes through each alien and draws it */
	iterate(*alien_head_list, alienStartGraphic);
}

/* Number to convert to milliseconds */
#define convToMilli 1000

/* Converts the time double to integer in millisecond form to output it in sa_time */
void outputCurrentTimeInGraphic(double time)
{	
	/* Finds time and outputs time for graphics */
	double timeInInt = time;
	timeInInt = timeInInt * convToMilli;
	sa_time((int)timeInInt);
}
/* Shoots a bolt if time is a whole number */
