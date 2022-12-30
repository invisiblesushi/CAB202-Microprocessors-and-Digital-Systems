#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "sprite.h"
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "usb_serial.h"
#include <math.h>

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

/*
* Defines used in system time calculations
*/
#define FREQUENCY 8000000.0
#define PRESCALER 1024.0

#define PI 3.14159265

#define BUFF_LENGTH 40

Sprite user;
//user ship
unsigned char space_bit[8] = {
  0b11000011,
  0b11000011,
  0b11000011,
  0b11011011,
  0b11011011,
  0b01111110,
  0b00111100,
  0b00011000,
};
Sprite alien;
//alien ship
unsigned char alien_bit[8] = {
	0b10000001,
	0b10011001,
	0b10111101,
	0b11111111,
	0b11111111,
	0b10111101,
	0b10011001,
	0b10000001,
};
Sprite rocket;
//missile rocket sprite
unsigned char rocket_bit[2] = {
	0b11000000,
	0b11000000,
};
Sprite aim;
unsigned char aim_bit[3] = {
	0b01000000,
	0b11100000,
	0b01000000,
};	

/*
* Global overflow count for TIMER1
*/
int gameover = 0;
int score = 0;
int lives = 3;
int dash_x;
int dash_y;
int dash_dx;
int dash_dy;
int dash_interval;
int aim_x;
int aim_y;
volatile unsigned long ovf_count = 0;

void setup();
void send_debug_string(char* string);
void timer_init(void);
double get_system_time(void);
void setup_border();
void setup_status();
void setup_menu();
void draw_centered(int y, char* string);
void user_process();
void spawn_alien();
void spawn_user();
void spawn_rocket();
void spawn_aim();
void fire_rocket();
void sprite_turn_to( Sprite* sprite, double dx, double dy );
void dash();
void dash_angle();
void dash_randomizer();
int sprite_step( Sprite* sprite );
int sprite_move_to( Sprite* sprite, double x, double y );
void sprite_turn( Sprite* sprite, double degrees );
bool sprite_move( Sprite* sprite, double dx, double dy );
int collision(Sprite s1, Sprite s2);
void restart();
void restart_screen();


int main (void){
	 setup();
	// Waiting for the debugger
    draw_string(11, 17, "Waiting for");
	draw_string(11, 24, "debugger...");
	
	show_screen();
	
	while(!usb_configured() || !usb_serial_get_control());
	
	send_debug_string("Initialised...Entering main loop");
	//main loop
	while(!gameover){
		clear_screen();
		setup_menu();
		show_screen();
		if((PINF >> 6) & 0b1 || (PINF >> 5) & 0b1 ){
			clear_screen();
			draw_centered(24,"Starting in 3");
			show_screen();
			_delay_ms(300);
			draw_centered(24,"Starting in 2");
			show_screen();
			_delay_ms(300);
			draw_centered(24,"Starting in 1");
			show_screen();
			_delay_ms(300);
			gameover = 1;
			
		}
		
	}
	while(gameover){
		clear_screen();
		
		timer_init();
		setup_border();
		setup_status();
		user_process();
		show_screen();
		
	}
	 
	 return 0;
}

// Set up the microcontroller
void setup(){
	set_clock_speed(CPU_8MHz);
	lcd_init(LCD_DEFAULT_CONTRAST);
	usb_init();
	//setup sw3/sw2 as inputs
	DDRF &= ~(1<<5);
	DDRF &= ~(1<<6);
	
	//setup joystick
	//up
	DDRD &= ~(1<<1);
	//down
	DDRB &= ~(1<<7);
	//left
	DDRB &= ~(1<<1);
	//right
	DDRD &= ~(1<<0);
}

void restart(){
	_delay_ms(500);
	void (*resetptr)( void ) = 0x0000;
	resetptr();
}
void restart_screen(){
	clear_screen();
	draw_centered(24,"Game Over");
	show_screen();
}

void setup_border(){
	
	//top main line 
	draw_line(1,8,83,8);
	//right main line
	draw_line(83,8,83,48);
	//left main line
	draw_line(0,8,0,48);
	//bottom main line
	draw_line(1,47,83,47);
}

void setup_status(){
	char buffer[BUFF_LENGTH];
	char buffer2[BUFF_LENGTH];
	char buffer3[BUFF_LENGTH];
	int second = (int)get_system_time() % 60;
	int minute = (int)get_system_time() / 60;
	
	sprintf(buffer3, "T:%02d:%02d", minute, second);
	sprintf(buffer2, "L:%d",lives);
	sprintf(buffer, "S:%d",score);
	draw_string(0,0,buffer);
	draw_string(25,0,buffer2);
	draw_string(45,0,buffer3);
}
void setup_menu(){
	draw_line(83,1,83,48);
	draw_line(1,1,1,48);
	draw_line(1,1,83,1);
	draw_line(1,47,83,47);

	draw_centered(3,"Alien Advance");
	draw_centered(11,"Vlad Jakovlev");
	draw_centered(19,"N9879188");
	draw_centered(31,"Press a button");
	draw_centered(39,"to continue...");
}

void spawn_user(){
	int user_x= (rand() % ( SCREEN_WIDTH - (user.width*2) )) + 1;
	int user_y= (rand() % ( SCREEN_HEIGHT - (user.height*2) -12 )) + 8;
	
	user.x = user_x;
	user.y = user_y;
	spawn_aim();
}

void spawn_alien(){
	int alien_x = ( (rand() % 83) - (user.width*2) - 1);
	int alien_y = ( (rand() % 39) - (user.height*2) - 8);
//	init_sprite(&alien, alien_x, alien_y, 8, 8, alien_bit);
//	draw_sprite(&alien);

	alien.x = alien_x;
	alien.y = alien_y;
}

void spawn_rocket(){
	int rocket_x = 0;
	int rocket_y = 0;
	
	rocket.x = user.x;
	rocket.y = user.y;
}
void spawn_aim(){
	aim_x = user.x;
	aim_y = user.y;
	
	aim.x = aim_x;
	aim.y = aim_y;
	
}
void fire_rocket(){
	sprite_step(&rocket);
	draw_sprite(&rocket);
}

void user_process(){
	
	spawn_user();
	spawn_alien();
	spawn_rocket();
	spawn_aim();
	
	while(1){
	clear_screen();
    //Up
    if ((PIND >> 1) & 0b1 ){
      if (user.y > 9){
        user.y--;
		//draw aim
		init_sprite(&aim, user.x+(user.width/3), user.y - 3, 3, 3, aim_bit);
		draw_sprite(&aim);
		//init rocket
		init_sprite(&rocket, user.x+3, user.y+2, 2, 2, rocket_bit);
		draw_sprite(&rocket);
		sprite_turn_to(&alien,user.x, user.y);
      }
    }
    //Down
    else if ((PINB >> 7) & 0b1 ){
      if (user.y < SCREEN_HEIGHT - 9){
        user.y++;
		//draw aim
		init_sprite(&aim, user.x+(user.width/3), user.y + user.height + 1, 3, 3, aim_bit);
		draw_sprite(&aim);
      }
    }
    //Left
    else if ((PINB >> 1) & 0b1 ){
      if (user.x > 1){
        user.x--;
		//draw aim
		init_sprite(&aim, user.x - 4, user.y + (user.height/3) - 1, 3, 3, aim_bit);
		draw_sprite(&aim);
      }
    }
    //Right
    else if ((PIND >> 0) & 0b1 ){
      if (user.x < SCREEN_WIDTH - 9){
        user.x++;
		//draw aim
		init_sprite(&aim, user.x + user.width + 1, user.y + (user.height/3) -1, 3, 3, aim_bit);
		draw_sprite(&aim);
      }
    }
	else if((PINB >> 5) & 0b1){
		fire_rocket();
	}
	draw_sprite(&aim);
	
	if((int)get_system_time == dash_interval){
		dash_angle();
		sprite_step(&alien);
		init_sprite(&alien, alien.x, alien.y, 8, 8, alien_bit);
		draw_sprite(&alien);
	}
	if ((alien.y > 9)&&(alien.y < SCREEN_HEIGHT - 8)&&(alien.x > 1)&&(alien.x < SCREEN_WIDTH - 8)){
		if(get_system_time() >= dash_interval){
		dash();
		sprite_step(&alien);
		draw_sprite(&alien);
		}
	}
	else{
		dash_randomizer();
		dash_angle();
		dash();
		sprite_step(&alien);
		draw_sprite(&alien);
		init_sprite(&alien, alien.x, alien.y, 8, 8, alien_bit);
	}
	if(lives == 0){
		restart_screen();
		_delay_ms(1000);
		if((PINB >> 5) &0b1 || (PINB >> 6) &0b1){
			score = 0;
			lives = 3;
			gameover = 1;
			spawn_user();
			TCNT1 = 0;
			ovf_count = 0;
			
		}
	}
	//draw user
	init_sprite(&user, user.x, user.y, 8, 8, space_bit);
    draw_sprite(&user);
	//alien
	init_sprite(&alien, alien.x, alien.y, 8, 8, alien_bit);
	draw_sprite(&alien);
	//draw rocket
	init_sprite(&rocket, rocket.x, rocket.y, 2, 2, rocket_bit);
	draw_sprite(&rocket);
	
	//draw playground
	setup_border();
	setup_status();
	show_screen();
	
	// if collision between user and alien is detected -1 life
	if (collision(user, alien)){
		lives--;
		spawn_user();
	}
	if(collision(alien,rocket)){
		score++;
		spawn_alien();
	}
	
	
	char buffer_user[BUFF_LENGTH];
	sprintf(buffer_user,"User X: %f User Y: %f",(double)user.x, (double)user.y);
	send_debug_string(buffer_user);
	

	}
	
}

void dash_randomizer(){
	
	int random_time = (rand() % 3) + 2;
	dash_interval = get_system_time() + random_time;
}

void dash(){
	int x = dash_x;
	int y = dash_y;

	double dx = x - alien.x;
	double dy = y - alien.y;
	double dist = sqrt( dx * dx + dy * dy );
	//  (f) Multiply dx by the desired speed, then divide it by the distance.
	dx = (dx * 0.8) / dist;
	//  (g) Do the same to dy.
	dy = (dy * 0.8) / dist;
	sprite_turn_to(&alien, dx, dy);

}

void dash_angle(){
	dash_x = user.x - ( 100 * (alien.x - user.x));
	dash_y = user.y - ( 100 * (alien.y - user.y));
}

int collision(Sprite s1, Sprite s2){
  int s1_top = round(s1.y);
  int s1_base = s1_top + s1.height - 1;
  int s1_left = round(s1.x);
  int s1_right = s1_left + s1.width - 1;

  int s2_top = round(s2.y);
  int s2_base = s2_top + s2.height - 1;
  int s2_left = round(s2.x);
  int s2_right = s2_left + s2.width - 1;

  if ( s1_base <= s2_top ) {
    return 0;
  }
  else if ( s1_top > s2_base ) {
    return 0;
  }
  else if ( s1_right < s2_left ) {
    return 0;
  }
  else if ( s1_left > s2_right ) {
    return 0;
  }
  else{
    return 1;
  }
}


void send_debug_string(char* string) {
     // Send the debug preamble...
	 char buffer[40];
     sprintf(buffer, "[DEBUG] @ %.2f : ", get_system_time());
     usb_serial_write(buffer, 17);

     // Send all of the characters in the string
     unsigned char char_count = 0;
     while (*string != '\0') {
         usb_serial_putchar(*string);
         string++;
         char_count++;
     }

     // Go to a new line (force this to be the start of the line)
     usb_serial_putchar('\r');
     usb_serial_putchar('\n');
 }
 
 // Initailise the timer
void timer_init(void){
    //Set to Normal Timer Mode using TCCR0B
    TCCR1B &= ~((1<<WGM02));

    //Set Prescaler using TCCR0B, using Clock Speed find timer speed = 1/(Clock Speed/Prescaler)
    //Prescaler = 1024
    //Timer Speed = 128 micro seconds
    //Timer Overflow Speed = 32678 micro seconds (Timer Speed * 256) - (256 since 8-bit timer)
	TCCR1B |= ((1 << CS12) | (1 << CS10));
	TIMSK1 |= (1 << TOIE1);
	
	sei();
}
 
 double get_system_time(void) {
    return (ovf_count * 65536 + TCNT1) * PRESCALER / FREQUENCY;
}

void draw_centered(int y, char* string){
  int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  //Find start position for center
  int x = 42 - ((i*5)/2);
  draw_string(x, y, string);
}

ISR(TIMER1_OVF_vect) {
    ovf_count++;
	send_debug_string("Timer overflowed");
}

void sprite_turn_to( Sprite* sprite, double dx, double dy ) {

	sprite->dx = dx;
	sprite->dy = dy;
}
int sprite_step( Sprite* sprite ) {

	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x += sprite->dx;
	sprite->y += sprite->dy;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}

int sprite_move_to( Sprite* sprite, double x, double y ) {
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x = x;
	sprite->y = y;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}

void sprite_turn( Sprite* sprite, double degrees ) {
	double radians = degrees * M_PI / 180;
	double s = sin( radians );
	double c = cos( radians );
	double dx = c * sprite->dx + s * sprite->dy;
	double dy = -s * sprite->dx + c * sprite->dy;
	sprite->dx = dx;
	sprite->dy = dy;
}


bool sprite_move( Sprite* sprite, double dx, double dy ) {
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x += dx;
	sprite->y += dy;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}
