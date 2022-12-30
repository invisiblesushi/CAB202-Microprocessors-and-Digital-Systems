#include <avr/io.h>
#include <util/delay.h>
#include "cpu_speed.h"

void setup_game_controller( void ) {
    //  (a) Set the CPU speed to 8MHz (you must also be compiling at 8MHz).
    set_clock_speed(CPU_8MHz);
    //  (b) Configure the data direction register for Port B to enable output
    //  to LED0 and LED1. The data direction for LED0 is controlled by Pin 2,
    //  while that for LED1 is controlled by Pin 3. No other pins should be
    //  affected.
    DDRB =  DDRB | 0b00001100;
    //  (c) Configure the data direction register for Port B to enable input
    //  from the Centre, Left, and Down switches on the joystick. These
    //  are controlled by Pins 0, 1, and 7 respectively. No other pins should be
    //  affected.
    DDRB = DDRB & 0b01111100;
    //  (d) Turn off LED0, LED1, and all other outputs connected to Port B, by
    //  clearing all bits in the Port B output register.
    PORTB &= ~(PORTB);
    //  (e) Configure the data direction register for Port D to enable input
    //  from the Right and Up switches on the joystick. These are controlled by
    //  Pins 0 and 1 respectively. No other pins should be affected by this
    //  operation.
    DDRD = DDRD & 0b11111100;
}

void process_game_controller( void ) {
    //  (f) Test the relevant pin in the Port D input register to determine
    //  if the Up switch is closed. If it is, turn on both LEDs without
    //  disrupting any other pins.
    if ((PIND >> 1) & 0b1 ){
      PORTB = PORTB | 0b00001100;
    }
    //  (g) OTHERWISE, test the relevant pin of the Port B input register
    //  to determine if the Down switch is closed. If it is, turn off both LEDs
    //  without affecting any other pins.
    else if ((PINB >> 7) & 0b1 ){
      PORTB = PORTB & 0b11110011;
    }
    //  (h) OTHERWISE, test the relevant pin of the Port B input register
    //  to determine if the Left switch is closed. If it is, turn on the left LED
    //  and turn off the right LED without affecting any other pins.
    else if ((PINB >> 1) & 0b1 ){
      PORTB = PORTB | 0b00000100;
      PORTB = PORTB & 0b11110111;
    }
    //  (i) OTHERWISE, test the relevant pin of the Port D input register
    //  to determine if the Right switch is closed. If it is, turn on the right LED
    //  and turn off the left LED without affecting any other pins.
    else if ((PIND >> 0) & 0b1 ){
      PORTB = PORTB | 0b00001000;
      PORTB = PORTB & 0b11111011;
    }
    //  (j) OTHERWISE, test the relevant pin of the Port B input register
    //  to determine if the Centre switch is closed. If it is, toggle both LEDs
    //  without affecting any other pins.
    else if ((PINB >> 0) & 0b1 ){
      //return register_value ^= (0b1 << pin);
      PORTB ^= (0b1 << 2);
      PORTB ^= (0b1 << 3);
    }
}


int main(void) {
    setup_game_controller();

    while ( 1 ) {
        process_game_controller();
        _delay_ms(100);
    }

    return 0;
}
