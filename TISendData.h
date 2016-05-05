/*
 * TISendList.h
 *
 *  Created on: Apr 25, 2016
 *      Author: Jonathan Lowe
 */

#ifndef TISENDDATA_H_
#define TISENDDATA_H_

void TISendList(unsigned char * charList, unsigned int charListLength);
void TISendVar(unsigned char charData, unsigned char targetVar);
void TISendVarGetCalc(unsigned char charData);
void TISendListGetCalc(unsigned char * charList);

#endif /* TISENDDATA_H_ */
