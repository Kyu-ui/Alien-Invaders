#include <stdio.h>
#include "output.h"
#include "debug.h"


#define colorInHex  0x70
#define typeInHex  0x7000
#define pointsInHex  0x1FF00000
#define colorShift  4
#define typeShift  12
#define pointsShift  20

/* Gets the significant digits for the Color in hex code and returns it in int form */
int getColor(unsigned int alienCode) 
{
	unsigned int isolatedBits = colorInHex;
	int shift = colorShift;
	/* "Turns on" the bits at 0x70 which is 1110000 in binary, then shift by 4 to get rid of zeros on the right */
	unsigned int color = (alienCode & isolatedBits) >> shift;
	/* If in debug, print corresponding debug statement */
	if (DEBUG) {
		printForDebugColor(isolatedBits, shift, alienCode, color);
	}
	return color;
}

/* Gets the significant digits for the Type in hex code and returns it in int form */
int getAlienType(unsigned int alienCode) 
{
	unsigned int isolatedBits = typeInHex;
	int shift = typeShift;
	/* "Turns on" the bits at 0x7000 which is 111000000000000 in binary, then shift by 12 to get rid of zeros on the right */
	unsigned int type = (alienCode & isolatedBits) >> shift;
	/* If in debug, print corresponding debug statement */
	if (DEBUG) {
		printFordebugType(isolatedBits, shift, alienCode, type);
	}
	return type;
}

/* Gets the significant digits for the points in hex code and returns it in int form */
int getPoints(unsigned int alienCode) 
{
	unsigned int isolatedBits = pointsInHex;
	int shift = pointsShift;
	/* "Turns on" the bits at 0x70 which is 11111111100000000000000000000 in binary, then shift by 20 to get rid of zeros on the right */
	unsigned int points = (alienCode & isolatedBits) >> shift;
	/* If in debug, print corresponding debug statement */
	if (DEBUG) {
		printForDebugPoints(isolatedBits, shift, alienCode, points);
	}
	return points;
}
