/*
 * CORDIC.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Michael Wong
 */

#ifndef PROJECT9_JL4GE_JYG4WX_CORDIC_H_
#define PROJECT9_JL4GE_JYG4WX_CORDIC_H_


#define CORDIC_X_EQUALS_Y

typedef enum {
	first, second, third, fourth
}Quadrant;

typedef struct {
	int angle;
	int hypotenuse;
} calculations;

typedef struct {
	unsigned int angle;
	unsigned int tilt;
} measurements;

typedef struct {
	int x;
	int y;
	int z;
}coordinates;

measurements getDisplaySetting(coordinates* position);
calculations findQuadrantAngle(int x, int y);
Quadrant getQuad(int x, int y);
unsigned int findTotalAngle(int quadAngle, Quadrant quad);
void setCoordinates(coordinates* position, int xc, int yc, int zc);


#endif /* PROJECT9_JL4GE_JYG4WX_CORDIC_H_ */
