#include <avr/io.h>
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



int main(void) {
int counter = 0;

    //initialise ports and LCD screen
    init_hardware();

    //initialiase timer
    timer_init();

    // Stop for 2 sec
    _delay_ms(10);

    // temp buffer
    char buffer[80];
    //double curr_time = 0.0;

    // Let's go through and collect the timer information
    while (1)
    {
      // Process a button press
      clear_screen();
      process_button();
      if (TCNT0 > counter){
        counter = TCNT0;
      }
      // Get the string to represent the time
      sprintf(buffer, "%d", counter);
      draw_string(20,10,buffer);

      // Show the screen
      show_screen();
      // Stop for 2 sec
      _delay_ms(100);
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
}
