#include <avr/io.h>
#include "cpu_speed.h"

int main(){
    set_clock_speed(CPU_8MHz);

    DDRB =  DDRB | 0b10001110;

    DDRF = DDRF & 0b10011111;



    DDRD = DDRD & 0b0000001;

    while(1){
        if((PINF  >> 6) & 0b1 ){
            PORTB = PORTB | 0b00000100;
        }
        else {
            PORTB = PORTB & 0b11111011;
        }
        if((PINF  >> 5) & 0b1 ){
            PORTB = PORTB | 0b00001000;
        }
        else {
            PORTB = PORTB & 0b11110111;
        }


        if((PINB >> 1 ) & 0b1){
            PORTB = PORTB | 0b00000100;
        }
        else {
            PORTB = PORTB & 0b11111011;
        }
        if((PIND >> 0) &0b1){
            PORTB = PORTB | 0b00001000;
        }
        else{
            PORTB = PORTB & 0b11110111;
        }

    }
return 0;
}
