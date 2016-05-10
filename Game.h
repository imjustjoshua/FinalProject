/*
 * Game.h
 *
 *  Created on: Apr 26, 2016
 *      Author: student
 */
#ifndef GAME_H_
#define GAME_H_

#define TARGETSIZE 		5
#define TARGETRADIUS 	2

#define MAXLOCATIONX 	94
#define MAXLOCATIONY 	62
#define MINLOCATIONX 	0
#define MINLOCATIONY 	0
#define MINTARGETX   	0
#define MINTARGETY   	0
#define MAXTARGETX   	94-TARGETSIZE
#define MAXTARGETY   	62-TARGETSIZE
#define MAXSCORE 	 	10
#define SCORETIME	 	10
#define SCORE_AMOUNT	1
#define SCORE_GOAL		4

typedef struct{
	unsigned char X;
	unsigned char Y;
} Point;

typedef struct {
	unsigned char score;
	Point location;
} Player;

Player us;
Point target;
Player them;

unsigned char scoreTimer;
unsigned char timer500ms;

//typedef enum{
//	False,True
//} Bool;

unsigned char gameData;
/* gameData contains bitwise information about the game
 * Bit7 (MSB)	= 1 if the game is running
 * Bit6			= 1 if this player won the game
 * Bit5			= 1 if the transmitter needs to send that the player scored
 * Bit4			= 1 if there is an Acceleromter error
 * Bit3			= 1 if there is a TI calculator error
 * Bit2			= Unused
 * Bit1			= 1 if there is a transmission error
 * Bit0 (LSB)	= 1 if there is a ___ error
 */

// Function headers
void moveUs(int togotoX, int togotoY);
void moveThem(unsigned char togotoX,unsigned char togotoY);
void initializeGame(void);
unsigned char randNum(void);
unsigned char randX(void);
unsigned char randY(void);
void moveTarget(void);
void targetCheck(void);
void initializePoint(Point *point, unsigned char X1,unsigned char Y1);
void updateDisplay(unsigned char inProgress);
void updateLocationOnTilt(void);

#endif /* GAME_H_ */
