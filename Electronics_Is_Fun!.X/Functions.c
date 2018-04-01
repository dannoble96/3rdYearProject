#include "Functions.h"

void init(void)
{
    OPTION_REG  = 0b00000111; //bit 6 sets EXT interrupt on falling edge
                             //set TMR0 source internal, prescale to TMR0, prescale to 1:256 
    INTCON = 0b10000000; //Set global interrupt enable
    
    TMR0 = 230; //preload tmr0 
    
    TRISB = 0b00000001;//set RB0 as input
    TRISA = 0x00;//set all as outputs
    
    PORTA = 0x00;//clear port
    PORTB = 0x00; //clear port
    
    min = 0x00;//set time to 0
    hour = 0x00;
    
    synced = 0;
    inputPin = 1;
}    

void interrupt extInterrupt (void)
{
    if (INTE && INTF){
       INTF = 0;
       
       if(synced == 1){
           secNo++;
           __delay_ms(150);//wait until data bit A comes through
           
           if(secNo > 38 && secNo < 45) //Hour information
           {
               hour = inputPin;
               hour = hour << 1; //shift to get ready for next bit
           }
           else if(secNo > 44 && secNo < 60)//minute information
           {
               min = inputPin;
               min = min << 1;  
           }
           __delay_ms(200); //wait until data bit B has past
           return;
       }
        
    }
    if (T0IE && T0IF){ //timer for 400ms
        timerCount++;
        
        if (timerCount == 7)
        {
            timerCount = 0;
            
            synced = 1;
        }
    }
        
}


void resetTime(void) //clear minute and second
{
    min = 0x00;
    hour = 0x00;
    secNo = 0;
}

void minMark(void) //The only time the signal will be off for over 300ms is at         
{                 //the minute mark.
    synced = 0; 
    do{
        
        if(inputPin == 0){
            INTCON |= 0b00100000; //turn on TMR0
        }
        else{
            INTCON = 0b10010000; //turn off TMR0
            TMR0 = 230;
            timerCount = 0; 
        }
        
    }while (!(inputPin == 0 && synced == 1));
}