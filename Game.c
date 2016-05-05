/*
 * Game.c
 *
 *  Created on: Apr 26, 2016
 *      Author: student
 */
#include "Game.h"
#include <stdlib.h>
#include <msp430.h>
#include "TISendData.h"
#include "TI_IO.h"
#include "CORDIC.h"

extern measurements angles;

void initializePoint(Point *point, unsigned char X1, unsigned char Y1) {
	point->X = X1;
	point->Y = Y1;
}
void initializeGame(void) {
	gameData = 0x1;
	us.score = 0;
	them.score = 0;
	initializePoint(&(us.location), MINLOCATIONX, MINLOCATIONY);
	initializePoint(&(them.location), MINLOCATIONX, MINLOCATIONY);
	initializePoint(&(target), randX(), randY());
	gameData = 0b10000000;
	scoreTimer = 0;
}

void moveUs(int togotoX, int togotoY) {
	while (togotoX >= 94) {
		togotoX -= 94;
	}

	while (togotoX < 0) {
		togotoX += 94;
	}

	while (togotoY >= 62) {
		togotoY -= 62;
	}
	while (togotoY < 0) {
		togotoY += 62;
	}

	us.location.X = togotoX;
	us.location.Y = togotoY;
}

void moveThem(unsigned char togotoX, unsigned char togotoY) {
	them.location.X = togotoX;
	them.location.Y = togotoY;
}

unsigned char randNum(void) {
	return (unsigned char) (rand() & 0x3f);
}

unsigned char randX(void) {
	return randNum() + 14;
}

unsigned char randY(void) {
	unsigned char dannyRandNum = randNum() - (TARGETSIZE >> 1);
	if (dannyRandNum > MAXTARGETY)
		dannyRandNum = MAXTARGETY;
	else if (dannyRandNum < MINTARGETY)
		dannyRandNum = MINTARGETY;
	return dannyRandNum;
}

void moveTarget(void) {
	us.score++;
	if (us.score == MAXSCORE) {
		gameData &= ~BIT7;
		gameData |= BIT6;
		return;
	}
	target.X = randX();
	target.Y = randY();
}

void targetCheck(void) {
	if (((us.location.X >= target.X) && (us.location.Y <= target.Y))
			&& !(them.location.X >= target.X)
			&& (them.location.Y <= target.Y)) {
		scoreTimer++;
		gameData |= BIT5;
		if (scoreTimer >= SCORETIME) {
			moveTarget();
			scoreTimer = 0;
		}
	}
}

void updateDisplay(void) {
	unsigned char dataToSend[7] = {us.location.Y, us.location.X, them.location.Y, them.location.X, target.Y, target.X, 1};

	while (!READ_TIZero || !READ_TIOne);
	TISendListGetCalc(dataToSend);
}

unsigned char getTiltMultiplier(void) {
	unsigned int tilt = angles.tilt;

	if (tilt < 400) {
		return 0;
	} else if (tilt < 1500) {
		return 1;
	} else if (tilt < 3000) {
		return 2;
	} else if (tilt < 4500) {
		return 3;
	} else if (tilt < 6000) {
		return 4;
	} else if (tilt < 7500) {
		return 5;
	} else if (tilt <= 9000) {
		return 6;
	}

	return 6;
}

void updateLocationOnTilt(void) {

	unsigned int angle = angles.angle;
	unsigned char multiplier = getTiltMultiplier();
	int dx = 0;
	int dy = 0;

	if ((angle < 2250) || (33750 <= angle && angle < 36000)) {
		// Direction N
		dy = -1 * multiplier;

	} else if (2250 <= angle && angle < 6750) {
		// Direction NE
		dy = -1 * multiplier;
		dx = 1 * multiplier;

	} else if (6750 <= angle && angle < 11250) {
		// Direction E
		dx = 1 * multiplier;

	} else if (11250 <= angle && angle < 15750) {
		// Direction SE
		dy = 1 * multiplier;
		dx = 1 * multiplier;

	} else if (15750 <= angle && angle < 20250) {
		// Direction S
		dy = 1 * multiplier;

	} else if (20250 <= angle && angle < 24750) {
		// Direction SW
		dy = 1 * multiplier;
		dx = -1 * multiplier;

	} else if (24750 <= angle && angle < 29250) {
		// Direction W
		dx = -1 * multiplier;

	} else if (29250 <= angle && angle < 33750) {
		// Direction NW
		dy = -1 * multiplier;
		dx = -1 * multiplier;
	}

	moveUs(us.location.X + dx, us.location.Y + dy);
}
