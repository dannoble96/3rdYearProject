/* 
 * File:   Function.h
 * Author: Home
 *
 * Created on 25 February 2018, 17:53
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

#include <xc.h>
#define _XTAL_FREQ 4000000
#define inputPin RB0

__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_ON);

signed char bcdMin;
signed char bcdHour;
signed char hourTens;
signed char hourUnits;
signed char minTens;
signed char minUnits;
int secNo;
int timerCount;
int synced;

void init (void);

void resetTime (void);

void minMark(void);

void setDisplayOutput(signed char number);

void updateTime(signed char bcdHour, signed char bcdMin);

void outputTime(void);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTION_H */

