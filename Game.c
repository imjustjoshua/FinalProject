/*
 * Game.c
 *
 *  Created on: Apr 26, 2016
 *      Author: student
 */
#include "Game.h"
#include <stdlib.h>
#include <msp430.h>
void initializePoint(Point *point, unsigned char X1,unsigned char Y1){
	point->X=X1;
	point->Y=Y1;
}
void initializeGame(){
	gameData=0x1;
	initializePoint(&(us->location), MINLOCATIONX, MINLOCATIONY);
	initializePoint(&(them->location), MINLOCATIONX, MINLOCATIONY);
	initializePoint(&(target),randX(),randY());
	gameData = 0b10000000;
}
void moveUs(unsigned char togotoX,unsigned char togotoY){
	us->location.X=togotoX;
	us->location.Y=togotoY;
}
void moveThem(unsigned char togotoX,unsigned char togotoY){
	them->location.X=togotoX;
	them->location.Y=togotoY;
}
unsigned char randNum(){
	return (unsigned char)(rand()&0x3f);
}
unsigned char randX(){
	return randNum()+14;
}
unsigned char randY(){
	unsigned char dannyRandNum = randNum()-(TARGETSIZE>>1);
	if(dannyRandNum>MAXTARGETY) dannyRandNum=MAXTARGETY;
	else if(dannyRandNum<MINTARGETY) dannyRandNum=MINTARGETY;
	return dannyRandNum;
}
void moveTarget(){
	us->score++;
	if(us->score==MAXSCORE){
		gameData&=~BIT7;
		gameData|=BIT6;
		return;
	}
	target.X=randX();
	target.Y=randY();
}
void targetCheck(){
	if(((us->location.X>=target.X)&&(us->location.Y<=target.Y))
			&&!(them->location.X>=target.X)&&(them->location.Y<=target.Y)){
		scoreTimer++;
		gameData|=BIT5;
		if(scoreTimer>=SCORETIME){
			moveTarget();
			scoreTimer=0;
		}
	}
}
