#include <avr/io.h>
#include <util/delay.h>
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <math.h>
#include "usb_serial.h"

//8MHz
#define FREQUENCY 8000000.0
#define PRESCALER_1 1024.0

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48

int game_over = 1;  //game_over
int lives = 3; //Number of lives
int score = 0;  //Score
int dash_x;
int dash_y;
int dash_time;

//Global overflow count for TIMER1 - keeps time increasing rather than wrapping
volatile unsigned long ovf_count = 0;

void init_hardware(void);
void start_screen(void);
void game_screen(void);
void draw_centred(int y, char* string);
void alien_process(void);
void process(void);
long get_system_time(void);
void game_status(void);
void init_timer_one(void);
void spawn_player(void);
void spawn_alien(void);
int collision(Sprite s1, Sprite s2);
void system_reset(void);
void dash_init(int num);
void dash_attack(void);
void dash_get_pos(void);
void key(void);
int sprite_step( Sprite* sprite );
void sprite_turn_to( Sprite* sprite, double dx, double dy );
void missile_init(void);
void missile_attack(void);
float get_system_ms(void);
void send_debug_string(char* string);

Sprite player;
//Player space craft
unsigned char space_craft[6] = {
  0b00011000, 0b00100100,
  0b00100100, 0b11111111,
  0b01111110, 0b00100100
};

Sprite gun;
int gun_x;
int gun_y;
char gun_pos = 'u'; //gun sight position to UP;
unsigned char gun_up_bitmap[2] = {
  0b01100000, 0b10010000
};
unsigned char gun_down_bitmap[2] = {
  0b10010000, 0b01100000
};
unsigned char gun_left_bitmap[4] = {
  0b01000000, 0b10000000,
  0b10000000, 0b01000000
};
unsigned char gun_right_bitmap[4] = {
  0b10000000, 0b01000000,
  0b01000000, 0b10000000
};

Sprite missile;
int missile_air = 0;
unsigned char missile_img[2] = {
  0b11000000, 0b11000000
};

Sprite alien;
int alien_dead = 0;
//alien ship
unsigned char alien_ship[8] = {
  0b00011000, 0b00111100,
  0b01111110, 0b11011011,
  0b11111111, 0b00011000,
  0b00100100, 0b01000010
};

int main(void) {
  // Setup the hardware
  init_hardware();

  draw_centred(10, "Waiting for");
  draw_centred(18, "debugger ...");
  draw_centred(30, "/dev/ttyACM0");
  draw_centred(38, "9600");

  show_screen();
  while(!usb_configured() || !usb_serial_get_control());

  char buffer_user[40];
  sprintf(buffer_user,"Greeting");
  send_debug_string(buffer_user);

  clear_screen();
  start_screen();
  show_screen();

  while(game_over){

    if((PINF  >> 6) & 0b1 || (PINF  >> 5) & 0b1){
      clear_screen();
      //Game start countdown 300ms interval
      draw_centred(10, "Starts in: 3");
      show_screen();
      _delay_ms(300);

      draw_centred(10, "Starts in: 2");
      show_screen();
      _delay_ms(300);

      draw_centred(10, "Starts in: 1");
      show_screen();
      _delay_ms(300);

      game_over = 0;
    }

    }
    if (!game_over){
      clear_screen();
      init_timer_one();


      spawn_alien();
      spawn_player();
      init_sprite(&gun, player.x + (player.width/3), player.y - 3, 4, 2, gun_up_bitmap);

      dash_init(0);

      while (1){
        process();
        show_screen();
      }
    }
}

void init_hardware(void) {
  set_clock_speed(CPU_8MHz);
  lcd_init(LCD_DEFAULT_CONTRAST);

  // Initialise the USB serial
  usb_init();

  //setup joystick, UP, DOWN, LEFT, RIGHT
	DDRD &= ~(1<<1);
	DDRB &= ~(1<<7);
	DDRB &= ~(1<<1);
	DDRD &= ~(1<<0);
  //SW1 SW2
  DDRF &= ~((1 << PF5) | (1 << PF6));

  //Turn on LCD led
  DDRC &= ~(1<<7);
	PINC |= (1<<7);
}

void init_timer_one(void){
  // Setup timer with overflow interrupts:
  // 1 - system time overflowing every 8.3s
  TCCR1B |= ((1 << CS12) | (1 << CS10));
  TIMSK1 |= (1 << TOIE1);
  //Clear timer1, set to 0
  TCNT1 = 0;

  // Enable interrupts globally
  sei();
}

void start_screen(void){
  draw_line(83,1,83,48);
  draw_line(1,1,1,48);
  draw_line(1,1,83,1);
  draw_line(1,47,83,47);

  draw_centred(3,"Alien Advance");
  draw_centred(11,"Daniel Chen");
  draw_centred(19,"N9879072");
  draw_centred(31,"Press a button");
  draw_centred(39,"to continue...");
}

void game_screen(){
  draw_line(83,8,83,48);
  draw_line(0,8,1,48);
  draw_line(0,8,83,8);
  draw_line(0,47,83,47);
}

void game_over_screen(){
  char buff[11];
  draw_centred(3,"GAME OVER");
  sprintf(buff, "Score : %d", score);
  draw_centred(11, buff);
  draw_centred(31,"Press a button");
  draw_centred(39,"to play again...");
}

void game_status(){
  char buff[11];
  int sec = (int)get_system_time() % 60;
  int minute = (int)get_system_time() / 60;
  //Draw timer
  sprintf(buff, "T:%02d:%02d", minute, sec);

  draw_string(49,0,buff);
  sprintf(buff, "S:%d", score);
  draw_string(0,0,buff);

  sprintf(buff, "L:%d", lives);
  draw_string(21,0,buff);

  show_screen();
}

void draw_centred(int y, char* string){
  int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  //Find start position for center
  int x = 42 - ((i*5)/2);
  draw_string(x, y, string);
}

void process(){
  char buffer_user[40];
  if (lives > 0) {

    int timer_diff = (get_system_ms() - (((int)get_system_time() % 60) * 1000 ));
    if ((timer_diff > 500 && timer_diff < 600) || (timer_diff > 0 && timer_diff < 100)){

      char heading[20];

      sprintf(buffer_user,"Player X: %.0f Y: %.0f",(double)player.x, (double)player.y);
      send_debug_string(buffer_user);

      if (gun.bitmap == gun_up_bitmap){
        sprintf(heading,"Heading : UP");
        send_debug_string(heading);
      }
      else if(gun.bitmap == gun_down_bitmap){
        sprintf(heading,"Heading : DOWN");
        send_debug_string(heading);
      }
      else if(gun.bitmap == gun_left_bitmap){
        sprintf(heading,"Heading : LEFT");
        send_debug_string(heading);
      }
      else if(gun.bitmap == gun_right_bitmap){
        sprintf(heading,"Heading : RIGHT");
        send_debug_string(heading);
      }
    }
    clear_screen();
    key();

    draw_sprite(&gun);
    init_sprite(&player, player.x, player.y, 8, 6, space_craft);
    draw_sprite(&player);

    if ( get_system_time() == dash_time ){
      dash_get_pos();
    }

    if (!alien_dead){
      // if border collision
      if (!((alien.y > 10) && (alien.y < SCREEN_HEIGHT - 9) && (alien.x > 2) && (alien.x < SCREEN_WIDTH - 9))){
        dash_init(0);
        dash_get_pos();
        dash_attack();
        sprite_step(&alien);
      }
      //if not collision
      else if ( get_system_time() >= dash_time ){
        dash_attack();
        sprite_step(&alien);
      }
      init_sprite(&alien, alien.x, alien.y, 8, 8, alien_ship);
      draw_sprite(&alien);
    }


    init_sprite(&missile, missile.x, missile.y, 2, 2, missile_img);
    if(missile.is_visible){
      missile_attack();
      sprite_step(&missile);
    }
    draw_sprite(&missile);

    game_screen();
    game_status();
    show_screen();
    show_screen();

    if (collision(player, alien)){
      lives--;
      spawn_player();
      init_sprite(&gun, player.x + (player.width/3), player.y - 3, 4, 2, gun_up_bitmap);
      init_sprite(&missile, -100, -100, 2,4, missile_img);
      missile_air = 0;
      sprintf(buffer_user,"Alien kills the player");
      send_debug_string(buffer_user);
    }
    else if (collision(alien, missile)){
      score++;
      alien.is_visible = 0;
      alien_dead = 1;
      spawn_alien();
      init_sprite(&missile, -100, -100, 2,4, missile_img);
      missile_air = 0;
      dash_init(2);
      dash_get_pos();
      sprintf(buffer_user,"Player kills the alien");
      send_debug_string(buffer_user);
    }
    if (!((missile.y > 9) && (missile.y < SCREEN_HEIGHT - 2) && (missile.x > 2) && (missile.x < SCREEN_WIDTH - 2))){
      missile_air = 0;
      init_sprite(&missile, -100, -100, 2,4, missile_img);
    }

}
  //Game over
  else{
    sprintf(buffer_user,"Game Over, Score: %.0f", (double)score);
    send_debug_string(buffer_user);
    clear_screen();
    game_over_screen();
    game_over = 1;
    show_screen();
    while(game_over == 1){
      //if sw1 or sw2 pressed, controller restart
      if((PINF  >> 6) & 0b1 || (PINF  >> 5) & 0b1){
        system_reset();
      }
    }
  }
}

void spawn_player(){
  srand(TCNT1 + (int)get_system_ms() % 1000);
  player.x = (rand() % ( SCREEN_WIDTH - (player.width*2) )) + 1;
  player.y = (rand() % ( SCREEN_HEIGHT - (player.height *2) -12 )) + 9;

  //If spawn overlap
  if(player.x < alien.x + 20 && player.x > alien.x - 20 && player.y < alien.y + 20 && player.y > alien.y - 20){
    spawn_player();
  }
}
void spawn_alien(){
  srand(TCNT1 + (int)get_system_ms() % 1000);
  alien.is_visible = 1;
  alien_dead = 0;
  alien.x = (rand() % ( SCREEN_WIDTH - (player.width*2) )) + 1;
  alien.y = (rand() % ( SCREEN_HEIGHT - (player.height *2) -12 )) + 9;

  //If spawn overlap
  if(player.x < alien.x + 20 && player.x > alien.x - 20 && player.y < alien.y + 20 && player.y > alien.y - 20){
    spawn_alien();
  }
}

//Key press
void key(){
  //UP
  if (((PIND >> 1) & 0b1 ) && ((player.y) > 9)){
      player.y += (-1);
      init_sprite(&gun, player.x + (player.width/3), player.y - 3, 4, 2, gun_up_bitmap);
  }
  //Down
  else if (((PINB >> 7) & 0b1 ) && ((player.y) < SCREEN_HEIGHT - 7)){
      player.y += 1;
      init_sprite(&gun, player.x + (player.width/3), player.y + player.height + 1, 4, 2, gun_down_bitmap);
  }
  //Left
  else if (((PINB >> 1) & 0b1 ) && ((player.x) > 1)){
      player.x += (-1);
      init_sprite(&gun, player.x - 3, player.y + (player.height/3) -1, 2,4,gun_left_bitmap);
  }
  //Right
  else if (((PIND >> 0) & 0b1 ) && ((player.x) < SCREEN_WIDTH - 9)){
      player.x += 1;
      init_sprite(&gun, player.x + player.width +1, player.y + (player.height/3) -1, 2,4,gun_right_bitmap);
  }
  //SW1
  if((PINF  >> 6) & 0b1){
    missile_init();
  }
}

void dash_init(int num){
  //Random 2-4 sec, rand() % 3 = 0 - 2
  srand(TCNT1 + (int)get_system_ms() % 1000);
  dash_time = get_system_time() + 2 + ((rand() % 3)) + num;
}

void dash_get_pos(){
  //Find x y dist between alien & player, times 100, get right angle.
  dash_x = player.x - ( 100 * (alien.x - player.x));
  dash_y = player.y - ( 100 * (alien.y - player.y));
}

void dash_attack(){
  int x = dash_x;
  int y = dash_y;

  double dx = x - alien.x;
  double dy = y - alien.y;
  double dist = sqrt( dx * dx + dy * dy );
  //Multiply dx by the desired speed, then divide it by the distance.
  dx = (dx * 1.0) / dist;
  dy = (dy * 1.0) / dist;
  sprite_turn_to(&alien, dx, dy);
}

void missile_init(){
  if (missile_air == 0){
    missile.is_visible = 0;
    if (gun.bitmap == gun_up_bitmap){
      gun_pos = 'u';
      init_sprite(&missile, player.x + (player.width/3) + 1, player.y - 3, 2, 2, missile_img);
    }
    else if(gun.bitmap == gun_down_bitmap){
      gun_pos = 'd';
      init_sprite(&missile, player.x + (player.width/3) + 1, player.y + player.height + 1, 2, 2, missile_img);
    }
    else if(gun.bitmap == gun_left_bitmap){
      gun_pos = 'l';
      init_sprite(&missile, player.x - 3, player.y + (player.height/2) -1, 2, 2, missile_img);
    }
    else if(gun.bitmap == gun_right_bitmap){
      gun_pos = 'r';
      init_sprite(&missile, player.x + player.width +1, player.y + (player.height/2) -1, 2, 2, missile_img);
    }
    missile_attack();
    missile_air =1;
    char buffer_user[20];
    sprintf(buffer_user,"Missile fired");
    send_debug_string(buffer_user);
  }
}

void missile_attack(){
  int x = 0;
  int y = 0;
  if (gun_pos == 'u'){
    y = -100;
    x = missile.x;
  }
  else if(gun_pos == 'd'){
    y = 100;
    x = missile.x;
  }
  else if(gun_pos == 'l'){
    x = -100;
    y = missile.y;
  }
  else if(gun_pos == 'r'){
    x = 100;
    y = missile.y;
  }

  double dx = x - missile.x;
  double dy = y - missile.y;
  double dist = sqrt( dx * dx + dy * dy );
  //Multiply dx by the desired speed, then divide it by the distance.
  dx = (dx * 2) / dist;
  dy = (dy * 2) / dist;
  sprite_turn_to(&missile, dx, dy);
}

//collision check between two sprites
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

//Retrun system time in S
long get_system_time(void) {
    long time = (ovf_count * 65536 + TCNT1) * PRESCALER_1 / FREQUENCY;
    return time;
}

//Return system time in ms
float get_system_ms(void) {
    float ms = ((ovf_count * 65536 + TCNT1) * PRESCALER_1 / FREQUENCY) * 1000;
    return ms;
}

void send_debug_string(char* string) {
  // Send the debug preamble...
	 unsigned char serial_buffer[100];
     sprintf(serial_buffer, "[DEBUG @ %.3f] ", get_system_ms()/1000);
     usb_serial_write(serial_buffer, 17);

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

//System reset, 500ms timer
void system_reset(){
  //Debounce
  _delay_ms(500);
  //Turn off lcd led
  DDRC &= ~(1<<7);
  PINC |= (1<<7);
  //reset
  void (*resetptr)( void ) = 0x0000;
  resetptr();
}

//Interrupt service routines
ISR(TIMER1_OVF_vect) {
    ovf_count++;
}

/*
 * ------------------------------------------------------------
 * File: cab202_sprites.c
 *
 * Simple character-based "graphics" library for CAB202, 2016 semester 2.
 *
 *	Authors:
 *	 	Lawrence Buckingham, Ben Talbot
 *
 *	$Revision:Sat Jul 23 23:22:19 EAST 2016$
 * ------------------------------------------------------------
 */

 /*
  *	Updates the sprite's location, adding the internally stored dx
  *	value to x and the internally stored dy to y.
  *
  *	Input:
  *		sprite: The ID of a sprite.
  *
  *	Output:
  *		Returns true if and only if the sprite occupies a different
  *		screen position after the move than before the move.
  */
int sprite_step( Sprite* sprite ) {
	int x0 = round( sprite->x );
	int y0 = round( sprite->y );
	sprite->x += sprite->dx;
	sprite->y += sprite->dy;
	int x1 = round( sprite->x );
	int y1 = round( sprite->y );
	return ( x1 != x0 ) || ( y1 != y0 );
}

/*
 *	Sets the stored displacement vector of a
 *	designated sprite to a new value.
 *
 *	Input:
 *		sprite: the ID of a sprite.
 *		dx, dy: the x- and y- components of the
 *					new displacement vector.
 */
void sprite_turn_to( Sprite* sprite, double dx, double dy ) {
	sprite->dx = dx;
	sprite->dy = dy;
}
