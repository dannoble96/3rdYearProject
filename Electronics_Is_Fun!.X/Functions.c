#include "Functions.h"

void init(void)
{
    OPTION_REG  = 0b01000111; //bit 6 sets EXT interrupt on rising/falling edge
                             //set TMR0 source internal, prescale to TMR0, prescale to 1:256 
    INTCON = 0b10000000; //Set global interrupt enable
    
    TMR0 = 230; //preload tmr0 
    
    TRISB = 0b00000001;//set RB0 as input all other portb o/p
    PORTB = 0x00; //clear port
    
    
    TRISA = 0x00;//set all porta as output
    PORTA = 0x00;//clear port
 
    bcdMin = 0x00;//set time to 0
    bcdHour = 0x00;
    
    synced = 0;
    INTCON = 0b10010000;//turn on exturnal interrupt
}    

void interrupt Interrupts (void)
{
    if (INTE && INTF){  //Check if interrupt flag is set
       INTF = 0;        //clear
       
       if(synced == 1){ //check if timing has been synced by minMark 
           secNo++;
           __delay_ms(150);//wait until data bit A comes through
           
           if(secNo > 38 && secNo < 45) //Hour information
           {
               bcdHour |= inputPin;    //Store bit value
               bcdHour =  bcdHour << 1; //shift to get ready for next bit
           }
           else if(secNo > 44 && secNo < 60)//minute information
           {
               bcdMin |= inputPin;
               bcdMin = bcdMin << 1;  
           }
           __delay_ms(200); //wait until data bit B has past
           return;
       }
        
    }
    if (T0IE && T0IF){ //timer for 400ms
        T0IF = 0; //clear flag
        timerCount++;
        
        if (timerCount == 7){
            timerCount = 0;
            
            synced = 1;
            
        }
        outputTime();
    }
        
}


void resetTime(void) //clear minute and second
{
    bcdMin = 0x00;
    bcdHour = 0x00;
    secNo = 0;
    return;
}

void minMark(void) //The only time the signal will be off for over 300ms is at         
{                 //the minute mark.
    synced = 0; 
    do{
        
        if(inputPin == 1 && synced == 0){
            INTCON |= 0b00100000; //turn on TMR0, left on after function ends 
        }
        else{
            INTCON = 0b10010000; //turn off TMR0
            TMR0 = 230;
            timerCount = 0; 
            synced = 1;
        }
        
    }while (!(inputPin == 1 && synced == 1));
    return;
}

void setDisplayOutput(signed char number)
{
    switch (number){
        case 0 : 
            PORTB = 0b11111100;
            break;
        case 1 :
            PORTB = 0b01100000;
            break;
        case 2 :
            PORTB = 0b11011100;
            break;
        case 3 : 
            PORTB = 0b11110010;
            break;
        case 4 :
            PORTB = 0b01100110;
            break;
        case 5 :
            PORTB = 0b10110110;
            break;
        case 6 :
            PORTB = 0b10111110;
            break;
        case 7 :
            PORTB = 0b11100000;
            break;
        case 8 :
            PORTB = 0b11111110;
            break;
        case 9 :
            PORTB = 0b1110011;
            break;
        case 10 :
            PORTB = 0b10001110;
            break;
        default :
            PORTB = 0b00000010;
            break;
                    
    }
}


void updateTime(signed char bcdHour, signed char bcdMin)
{
    hourTens = (bcdHour & 0b00110000); //extract hour tens
    hourUnits = (bcdHour & 0b00001111); //extract hour units
    minTens = (bcdMin & 0b00110000);// extract minute tens
    minUnits = (bcdMin & 0b00001111);//extract minute minutes
    
    switch(hourTens){
        case 0b00000000 :
            hourTens = 0;
            break;
        case 0b00010000 : //10
            hourTens = 1;
            break;
        case 0b00100000 : //20
            hourTens = 2;
            break;
        default :
            hourTens = 10;//fault state
            break;
    }
    
    if (hourUnits < 10)
    {
        hourUnits = hourUnits;
    }
    else
    {
        hourUnits = 10; //Fault state
    }
    
    switch(minTens){
    case 0b00000000 :
        minTens = 0;
        break;
    case 0b00010000 : //10
        minTens = 1;
        break;
    case 0b00100000 : //20
        minTens = 2;
        break;
    default :
        minTens = 10;//fault state
        break;
    }
        
        
    if (minUnits < 10)
    {
        minUnits = minUnits;
    }
    else
    {
        minUnits = 10; //Fault state
    }
}
void outputTime(void)
{
    //RA0=hourTens RA1=minTens RA2=minUnits RA3=hourUnits
    PORTA = 0b0001;
    setDisplayOutput(hourTens);
    PORTA = 0b0010;
    setDisplayOutput(minTens);
    PORTA = 0b0100;
    setDisplayOutput(minUnits);
    PORTA = 0b1000;
    setDisplayOutput(hourUnits);
}
