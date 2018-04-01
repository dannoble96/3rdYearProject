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

int min;
int hour;
int secNo;
int timerCount;
int synced;

void init (void);

void resetTime (void);

void minMark(void);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTION_H */

