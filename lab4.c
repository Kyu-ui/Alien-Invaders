#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "altmem.h"
#include "linkedlistlib.h"
#include "invaders.h"
#include "code.h"
#include "output.h"
#include "debug.h"
#include "motion.h"
#include "lab4.h" 
#include "Memory.h"

#define framerateInMicro 31250
#define fourSecInMicro 4000000
/* The number to convert a second to a millisecond */
#define convToMilli 1000
#define numTrue 1
#define saucerType 3
#define posXbound 39
#define negXbound -39

/* ----------------------------------------------- */
/* Helper functions for Graphics */
/* ----------------------------------------------- */

/* Checks if a double is the same value as a whole num in int form, then returns its corresponding int form */
int isWholeNum(double num) {
	/* checks to see if a number rounded up and down are the same, this only happens if a double is equivalent to a integer */
	/* Returns true if so, if not then returns false */
	if (ceil(num) == floor(num)) {
		return numTrue;
	}
	return 0;
}
	
/* Shoots a bolt if time is a whole number */
void check_bolt(double time)
{
	/* Checks if the double is a whole num and if so fires a bolt */
	if (isWholeNum(time)) {
		sa_bolt(0.0);
	}
}



/* ----------------------------------------------- */
/* Linked list helper functions */
/* ----------------------------------------------- */

/* Adds the points to score, changing it */
void addPointsToScore(void *alienStruct)
{	
	/* Dereferences Alien and Structure pointers */
	Alien *alienStructPtr = alienStruct;
	Sim *simStructPtr = alienStructPtr->simStructPtr;
	/*Changes the score to add on the amount of points from the alien structure */
	simStructPtr->score = simStructPtr->score + alienStructPtr->points;
	/* Changes the score in graphics mode */
	if(GRAPHICS)sa_score(simStructPtr->score);
}

/* Moves aliens */
void moveRegularAliens(void *alienStruct)
{
	/* Dereferences Alien  pointers */
	Alien *alienStructPtr = alienStruct;
	/* updates the values of the aliens */
	updateMotion(alienStructPtr);
}

/* Sorts aliens by altitude */
int compare(void *alien1, void *alien2)
{
	/* dereferences alien pointers */
	Alien *Alien1 = alien1;
	Alien *Alien2 = alien2;
	/* returns true for higher altitude */
	if (Alien1->Y > Alien2->Y)
	{
		return numTrue;
	}
	return 0;
}

/* Checks if the alien is over the base. If it is, prints out message and adds correct amount of points */
int alienOverBase(void *alienStruct, void *helper)
{
	/* Dereferences Alien and Structure pointers */
	Alien *alienStructPtr = alienStruct;
	Sim *simStructPtr = alienStructPtr->simStructPtr;
	/* Will always return 0 if the alien isn't above the base and will only do implicit conversion if alien is at a whole num */
	if (isWholeNum(alienStructPtr->X) && (int) alienStructPtr->X == (int) simStructPtr->Xpos && isWholeNum(simStructPtr->elapsedTime)) {
		
		/* Prints output for bolt hitting */
		printForBoltHit(alienStruct);
		/* adds score from alien dead */
		addPointsToScore(alienStruct);
		return numTrue;
	}
	
	return 0;
}

/* Checks if alien is out of bounds */
int outOfBounds(void *alienStruct, void *helper) 
{
	/* Dereferences Alien  pointers */
	Alien *alienStructPtr = alienStruct;
	/* If alien is a saucer and its out of bounds, print saucer escape and return if a saucer has escaped or not */
	if ((alienStructPtr->type == saucerType) && (alienStructPtr->X >= posXbound || alienStructPtr->X <= negXbound))
	{
		printForSaucerEscape(alienStruct);
		return numTrue;
	}
	return 0;
}

/* Function to check if an alien is on the ground */
int onGround(void *alienStruct, void *helper)
{
	/* Dereferences Alien  pointers */
	Alien *alienStructPtr = alienStruct;
	/* Checks if alien is on the ground, if it is prints touchdown and returns that aliens have touched or not touched the ground. clips aliens to 0*/
	if (alienStructPtr->Y <= 0.0)
	{	
		
	if(DEBUG) printf("The alien is on ground here and the y value is: %lf and the x value is %lf\n", alienStructPtr->Y, alienStructPtr->X);

		printForTouchDown(alienStruct);
		return numTrue;
	}
	return 0;
}

/* CritieriaFunction to always return true */
int alwaysTrue(void *alienStruct, void *helper)
{
	return numTrue;
}

/* Critieria function to always return true so it only runs through a single alien. Used to Print tab header */
int printTabHeaderCriteria(void *alienStruct, void *helper)
{
	/* Prints out header for tab output */
	tabularTextOutputHeader(alienStruct);
	/* Always returns true since we only want this running once through the list */
	return numTrue;
}


/* ----------------------------------------- */
/* Main functions for the sim loop */
/* ----------------------------------------- */

/* updates elapsedTime in simStruct */
void updateTime(void *simStruct) 
{
	/* Dereferences Structure pointers */
	Sim *simPtr = simStruct;
	if(DEBUG)printf("Testing the time is incremented: %lf\n", simPtr->elapsedTime);
	/* Updates elapsedTime in sim struct */
	simPtr->elapsedTime = simPtr->elapsedTime + simPtr->deltaTvar;
}

/* Deletes if an alien ius over the base */
void fightBack(void **alien_head_list)
{	
	/* function that checks if alien is over the base */
	CriteriaFunction overBase = &alienOverBase;	
	/* function that frees alien */
	ActionFunction freeAlien = &free_alien;
	/* deletes aliens over the base */
	deleteSome(alien_head_list, overBase, NULL, freeAlien, TEXT);

}



/* Deletes aliens if needed */
void deleteAliensMaybe(void **alien_head_list)
{
	/* Function to check if alien is out of bounds */
	CriteriaFunction outOfBoundFunc = &outOfBounds;
	/* Function for freeing alien */
	ActionFunction freeAlien = &free_alien;
	
	/* Deletes if aliens saucers are out of bounds */
	deleteSome(alien_head_list, outOfBoundFunc, NULL, freeAlien, TEXT);  

}

/* Prints only the tab header */
void printTabHeader(void **alien_head_list)
{
	/* Function to get an alien in order to get the ET */
	CriteriaFunction onetabHeader = &printTabHeaderCriteria;
	/* Will only run on a single alien to print out the header */
	any(*alien_head_list, onetabHeader, NULL);
}

/* Prints tab output */
void printGame(void **alien_head_list)
{
	/* function to print tab output */
	ActionFunction printOutput = &tabularTextOutput;
	/* Prints tab output */	
	printTabHeader(alien_head_list);
	/* prints tab output for all aliens */
	iterate(*alien_head_list, printOutput);
}


/* Prints the tab output if whole number is reached in ET */
void maybePrintTable(void **alien_head_list, void *simStruct, int alienTouchGround)
{
	
	/* Dereferences Structure pointers */
	Sim *simStructPtr = simStruct;
	/* Checks if ET is a whole num */
	if(isWholeNum(simStructPtr->elapsedTime))
	{
		/* If alien touched the ground, dont print as it would double print at the end */
		if (!alienTouchGround)
		{	
			/* Prints tab output */
			printGame(alien_head_list);	
		}
	}
}

/* moves the simulation, returns if any aliens hit the ground*/
int moveSimulation(void **alien_head_list)
{	
	/* Initialize variable for aliens touch the ground yet */
	int alienTouchGround = 0;
	/* Function to move aliens */
	ActionFunction moveTime = &moveRegularAliens;
	/* Functions to check if any aliens on the ground */
	CriteriaFunction ground = &onGround;
	/* Returns true if any aliens are on the ground */
	alienTouchGround = any(*alien_head_list, ground, NULL);
	/* Moves all aliens if an alien hasnt touched the ground */
	if(!alienTouchGround)iterate(*alien_head_list, moveTime);

	
	/* returns if aliens hit the gorund */
	return alienTouchGround;
}

/* Prints what happens when the game ends */
void printEndGame(void **alien_head_list, int alienTouchGround, void *simStruct)
{
	/* Function to print the output */	
	ActionFunction printOutput = &tabularTextOutput;
	/* Prints the header of tabular output */
	printTabHeader(alien_head_list);
	/* Prints all tabular stuff for each alien */
	iterate(*alien_head_list, printOutput);
	/* Prints either the defeat or victory message */
	printDefeatOrVictory(alien_head_list, simStruct, alienTouchGround);
	/* Prints score at the end */
	printScore(simStruct);
}
/* Gets rid of all aliens in the list */
void clearAliens(void **alien_head_list)
{	
	/* initialized alien cleared var */
	int aliensCleared = 0;	
	/* Function to always return true */
	CriteriaFunction True = &alwaysTrue;
	/* Function to free aliens */
	ActionFunction freeAliens = &free_alien;
	/* Prints message of clearing starting */
	printAliensClearingStart();
	/* deletes all aliens in list, returning how many deleted */
	aliensCleared = deleteSome(alien_head_list, True, NULL, freeAliens, TEXT);
	/* Prints how many aliens cleared */
	printAliensCleared(aliensCleared);
}

/* Refreshes the graphic, clears and draws new frame according to what correct frame should be drawn */
void refreshGraphics(void **alien_list_head, void *simStruct) 
{
	/* Dereferences  Structure pointers */
	Sim *simStructPtr = simStruct;
	/* Clears screen, then draws alien and base */
	sa_clear();
	/* Draws aliens and base */
	drawAliens(alien_list_head);	
	sa_base(0.0);
	/* outputs score */
	sa_score(simStructPtr->score);
	/* Calls to see if should fire a bolt */
	check_bolt(simStructPtr->elapsedTime);
	/* Outputs the time in sa_time */
	outputCurrentTimeInGraphic(simStructPtr->elapsedTime);
	/* Refreshes */
	sa_refresh();
	/* Sleeps for a single framerate */
	microsleep(framerateInMicro);
}

/* Runs the endGame routine for 4 more seconds after alien touchdonw */
void runEndGame(void **alien_list_head, void *simStruct) 
{
	/* sets the ending of the while loop to four seconds in microseconds */
	double fourMicro = fourSecInMicro;
	double start = 0;
	while (start <= fourMicro) {
		/* calls refreshGraphics to output graphics until whileloop ends, which is in 4 seconds */
		refreshGraphics(alien_list_head, simStruct);
		start = start + framerateInMicro;
	}
}

/* Starts the sim loop in graphics or in text mode */
void startSimLoop(void **alien_head_list)
{	
	/* If in graphics, initializes the graphics */
	if (GRAPHICS) drawAliens(alien_head_list);
	/*If in text, prints the list once */
	if(TEXT) printGame(alien_head_list);
}

/* Prints or runs end game whether in text or graphics mode */
void doEndGame(void **alien_head_list, void *simStruct, int alienTouchGround)
{
	/* If in graphics, run endGame simulation */	
	if (GRAPHICS) runEndGame(alien_head_list, simStruct); 
	/* If in text mode, prints chart of statistics */
	printEndGame(alien_head_list, alienTouchGround, simStruct);
}

/* Starts the simloop for a the game, whether in graphics or in text */
void simloop(unsigned int *alien, double *x, double *y, double *VX, double *VY, double *time, void *simStruct, void **alien_head_list, int alienTouchGround)
{	
	/*Either prints alien list or starts graphics depending on mode */
	startSimLoop(alien_head_list);

	while(*alien_head_list != NULL && !alienTouchGround) {

		/*Moves aliens, returns true if an alien touches the ground */
		alienTouchGround = moveSimulation(alien_head_list);

		/* Functions below will not run if an alien touches the ground */
		/*Updates time for the sim loop */
		if(!alienTouchGround)updateTime(simStruct);
		/* deletes aliens if they are over the bolt */
		if(!alienTouchGround)fightBack(alien_head_list);
		/*Deletes aliens if they are perhaps a saucer flying outside of the game */
		if(!alienTouchGround)deleteAliensMaybe(alien_head_list);		
		/* Prints table if we are in text mode and in a whole number increment */
		if(!alienTouchGround)maybePrintTable(alien_head_list, simStruct, alienTouchGround);		

		
		/* Refreshes the graphics each frame */
		if (GRAPHICS) refreshGraphics(alien_head_list, simStruct);
	}
	/* Runs end game */
	doEndGame(alien_head_list, simStruct, alienTouchGround);
	/* Clears all aliens from the list */
	clearAliens(alien_head_list);
}

/* ------------------------------------------- */
/* Helper functions for read loop */
/* ------------------------------------------- */


/* Creates aliens ans assign values */
void createSingleAlien(unsigned int alien, double X, double Y, double VX, double VY, void *alienStructPtr, void *simPtr)
{
	/* Dereferences Alien and Structure pointers */
	Alien *alienStruct = alienStructPtr;
	/* Assigns all values returned from scanf to a single alien struct */
	alienStruct->X = X;
	alienStruct->alien = alien;
	alienStruct->Y = Y;
	alienStruct->VX = VX;
	alienStruct->VY = VY;
	alienStruct->points = getPoints(alien);
	alienStruct->color = getColor(alien);
	alienStruct->type = getAlienType(alien);
	alienStruct->simStructPtr = simPtr;
}



/* PUTS ALIEN IN THE LINKED LIST */
void putAliensInList(unsigned int alien, double X, double Y, double VX, double VY, void **alien_list_head, void *simPtr) 
{
	/* Initializes variable to check insert */
	int checkInsert;
	/* Comparison function for insert */
	ComparisonFunction compare_alien = &compare;
	/* allocates a mem for a alienStruct */
	Alien *newAlienStructPtr = allocateMem();


/* IF STATEMENTS BELOW TO CHECK IF THE ALIEN POINTER ISNT NULL IN ORDER TO SUCCESSFULLY ALLOCATE THE ALIEN IN THE LIST */

	/* Creates a single alien struct */
	if (newAlienStructPtr != NULL) createSingleAlien(alien, X, Y, VX, VY, newAlienStructPtr, simPtr);

	/* Inserts alien in list */
	if (newAlienStructPtr != NULL) checkInsert = insert(alien_list_head, newAlienStructPtr, compare_alien, TEXT);
	/* If failed to insert, frees the alien */
	if(newAlienStructPtr != NULL && !checkInsert) free_alien(newAlienStructPtr);
}
/* Creates the simStruct and returns the pointer to the simStruct */
void *createSimStruct() 
{
	/* Static so it can be returned as a pointer */
	/* Initializes elapsed time, alien list, xpos, score, and deltaT to their respesctive values in order */

	static Sim simStruct = {0,NULL,0,0,deltaT, NULL, NULL};
	
	/* Initializes input and output to stdin and stdout */
	simStruct.input = stdin;
	simStruct.output = stdout;
	
	/* returns the simStruct */
	return &simStruct;
}

/* Closes file and output diagnostic */
void closeFile(FILE *filePointer, int argc, char *argv[])
{
	/* Closes file and prints diagnostic */
	fclose(filePointer);
	printf("Closed input file %s\n", argv[argc - 1]);
}

/* Starts the loop of reading in values of aliens from file and outputting them. */
/* Uses code from Neil Kirby */
void readLoop(unsigned int *alien, double *X, double *Y, double *VX, double *VY, double *elapsedtime, FILE *filePointer, int argc, char *argv[])
{	

	/* Initialize variable for scanf */
	int returnValueOfScan;	

	/* value for the simloop to stop simloop if the aliens reach ground */
	int alienTouchGround = 0;

	/* Creates the alien head pointer */
	void *alien_list_head = NULL;

	/* Craetes the simStruct pointer */
	void *simStructPtr = createSimStruct();

	

	/* Runs as long as the returnvalue is 5, and stores the items into variables */
	while (5 == (returnValueOfScan = fscanf(filePointer, "%X %lf %lf %lf %lf", alien, X, Y, VX, VY))) 
	{	
		/* Puts values in scanf into a single alien struct */
		putAliensInList(*alien, *X, *Y, *VX, *VY, &alien_list_head, simStructPtr);
	}
	/* Close file input */
	closeFile(filePointer, argc, argv);

	/* Prints incidental message that scanf terminates */
	if(TEXT) printScanTerminated(returnValueOfScan);
	/*runs sim loop */
	 simloop(alien, X, Y, VX, VY, elapsedtime, simStructPtr, &alien_list_head, alienTouchGround);
}
/* Initializes file and return a file pointer */
FILE *fileInit(int argc, char *argv[])
{
	/* Should only open the second command line since the second will always be the file to open an alien */
	FILE *filePointer = fopen(argv[1], "r");
/* Prints diagnostic message accordinly */	
	if (filePointer == NULL)
	{
		fprintf(stderr, "ERROR: Unable to open %s for input\n", argv[1]);
	}
	else 
	{
		printf("DIAGNOSTIC: Successfully opened %s for input\n", argv[1]);
	}
	
	
	return filePointer;


}

	
/* Starts the game, either in text or graphics mode. Prints out runtime */
int main(int argc, char *argv[])
{

	/* Creates filePOinter from cmd args */
	FILE *filePointer = fileInit(argc, argv);

	/* Initialization of variables */
	unsigned int alien = 0;
	double X, Y, VX, VY, startTime, elapsedtime = 0;

	/* Keeps track of the startTime */
	startTime = now();
	/* Short circuits, if in text mode will not sa_initialize */
	if( TEXT || (GRAPHICS && sa_initialize()) ) {
		
		/* Only does stuff if the there is a file to read in aliens */
		if (filePointer != NULL) {

			/* Reads in aliens for a simulation */
			readLoop(&alien, &X, &Y, &VX, &VY, &elapsedtime, filePointer, argc, argv);
			/* Stops graphics */
	       		if (GRAPHICS) sa_teardown();
		}
	}
	
	/* prints total runtime */
	printRuntime(startTime);

	return 0;
}
