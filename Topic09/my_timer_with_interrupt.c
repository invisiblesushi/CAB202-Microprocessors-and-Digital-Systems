#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>


#include "sprite.h"
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"



// Debouncing for PINF
void debounce_PINF(int pin_number);

// Process button presses
void process_button(void);

// Initialise the hardware
void init_hardware(void);

// Initialise the timer
void timer_init(void);


// My global variable for overflowing, set it to volatile
volatile int overflow_count;




///////////////////// MAIN PROGRAM /////////////////////////////////////
int main(void) {


    //initialise ports and LCD screen
    init_hardware();

    //initialiase timer
    timer_init();

    // Stop for 2 sec
    _delay_ms(2000);

    // temp buffer
    //double curr_time = 0.0;

    // Let's go through and collect the timer information
    while (1)
    {
    }


	return 0;
}

///////////////////// HELPER FUNCTIONS ////////////////////////////////

// Initialise the hardware
void init_hardware(){
  // Set the clock speed
  set_clock_speed(CPU_8MHz);

  // Use the switch as an input, PIN5
  DDRF   &= ~(0b1<<5);

  // Use the LED as anout output, PIN2
  DDRB  |= 0b1<<2;

  // Intialise the LCD
  lcd_init(LCD_DEFAULT_CONTRAST);
  clear_screen();
  draw_string(10,10,"Hello");
  show_screen();
}

// Process the button presses
void process_button(void){
  // Check if the button was pressed
  if ((PINF >> 5) & 0b1){
    // Do something to handle the bouncing of the switch
    debounce_PINF(5);

    // Set the LED ON!
    PORTB     ^= 0b1<<2;
  }
}

// Deal with bouncing on PINF
void debounce_PINF(int pin_number){
  // Wait for 100ms
  _delay_ms(100);

  // While the button is being pressed, wait..
  while ((PINF>>pin_number) & 0b1);

  // Wait for 100ms
  _delay_ms(100);
}

// Initailise the timer
void timer_init(void)
{
  // ???? 0101   Prescaler set to 1024

  //Set to Normal Timer Mode using TCCR0B
  TCCR0B &= ~((1<<WGM02));

  //Set Prescaler using TCCR0B, using Clock Speed find timer speed = 1/(Clock Speed/Prescaler)
  //Prescaler = 1024
  //Timer Speed = 128 micro seconds
  //Timer Overflow Speed = 32678 micro seconds (Timer Speed * 256) - (256 since 8-bit timer)
  TCCR0B |= (1<<CS02)|(1<<CS00);
  TCCR0B &= ~((1<<CS01));

  // Enable the interrupt and that the global interrupts are enabled.
  TIMSK0 |= (0b1 << TOIE0);
  sei();

}


// Set the interrupt service routine for timer 0
ISR(TIMER0_OVF_vect){
  //
  char buffer[80];

  // Increment our internal overflow counter
  overflow_count++;

  // Get the string to represent the time
  clear_screen();
  sprintf(buffer, "%d", overflow_count);
  draw_string(20,10,buffer);

  // Show the screen
  show_screen();

}
