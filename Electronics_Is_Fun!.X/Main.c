/* 
 * File:   Main.c
 * Author: Home
 *
 * Created on 25 February 2018, 17:52
 */

#include "Functions.h"
/*
 * 
 */
int main() {
    
    init();
    resetTime();
    minMark();//wait for 1st minute to past
    
    while(1){
        
        INTCON = 0b10010000; // turn on interrupt
        
    }

    
    
    return;
}

