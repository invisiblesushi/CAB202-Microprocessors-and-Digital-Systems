#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// Game state.
#define DELAY (10)
bool game_over = false;   //true when game over
bool game_pause = true;  //true when game pause
bool game_respawn = false;  //true when game respawn
bool update_screen = true;

//set start lives and start score
int lives = 10; //Number of lives
int score = 0;  //Score

//diamond stats
#define MAX_DIAMOND 10
#define diamond_big_w 5
#define diamond_big_h 5
sprite_id diamond[MAX_DIAMOND];
int diamond_count = MAX_DIAMOND;

char * diamond_big_img =
/**/	"  C  "
/**/	" CCC "
/**/	"CCCCC"
/**/	" CCC "
/**/	"  C  ";

//Ship stats
sprite_id ship;
#define SHIP_WIDTH 7
#define SHIP_HEIGHT 5
char * ship_image =
/**/	"   ^   "
/**/	"  | |  "
/**/	"  | |  "
/**/	" /|^|\\ "
/**/	"/_|||_\\";

//missile stat
sprite_id missile;
char * missile_img =
/**/	"^"
/**/	"|";

//Timer stat
timer_id my_timer;
int second = 0;
int minute = 0;
int total_sec = 0;
double timer_init = 0;

//Draw pause/help screen
void pause_screen(){
  int boxx = 40;
  int boxy = 12;

  int right = (screen_width() - boxx) / 2;
  int top = (screen_height() - boxy) / 2;
  int left = screen_width() - right;
  int bot = screen_height() - top;

  draw_line( left, top, right, top, '#');
  draw_line( left, top + 5, right, top + 5, '#');
  draw_line( left, top + 11, right, top + 11, '#');
  draw_line( right, top, right, bot, '#');
  draw_line( left, bot, right, bot, '#');
  draw_line( left, top, left, bot, '#');

  draw_formatted( screen_width()/2 - (19/2), top + 1,
      "CAB202 Assignment 1"    );
  draw_formatted( screen_width()/2 - (20/2), top + 2,
      "The Diamonds of Doom"    );
  draw_formatted( screen_width()/2 - (11/2), top + 3,
      "Daniel Chen"    );
  draw_formatted( screen_width()/2 - (8/2), top + 4,
      "N9879072"    );
  draw_formatted( screen_width()/2 - (8/2), top + 6,
      "Controls"    );
  draw_formatted( screen_width()/2 - (16/2), top + 7,
      "q         : quit"   );
  draw_formatted( screen_width()/2 - (16/2), top + 8,
      "h         : help"    );
  draw_formatted( screen_width()/2 - (27/2), top + 9,
      "a / d     : move left/right"    );
  draw_formatted( screen_width()/2 - (24/2), top + 10,
      "Space     : shoot bullet"    );
  draw_formatted( screen_width()/2 - (22/2), top + 12,
      "Press a key to play..."    );

  show_screen();
}

//Draw game over screen
void game_over_screen(){
  draw_formatted( screen_width()/2 - (9/2), screen_height()/2,
      "GAME OVER"    );
  draw_formatted( screen_width()/2 - (9 /2), screen_height()/2 + 1,
      "Score: %1d",
      score
  );
  draw_formatted( screen_width()/2 - (15/2), screen_height()/2 + 3,
      "y to play again"    );
  draw_formatted( screen_width()/2 - (9/2), screen_height()/2 + 4,
      "n to exit"    );
  show_screen();
}

//Draw border
void draw_border( void ) {
  int left = 0;
  int top = 0;
  int right = screen_width() - 1;
  int bot = screen_height() -1;
  draw_line( left, top, right, top, '#');
  draw_line( left, top + 2, right, top + 2, '#');
  draw_line( right, top, right, bot, '#');
  draw_line( left, bot, right, bot, '#');
  draw_line( left, top, left, bot, '#');
}

//Draw status
void draw_status() {
  draw_formatted( (screen_width()/2) - (56 /2), 1,
      "Lives: %10d, Score: %10d, Time: %10d:%d",
      lives, score, minute, second
  );
}

//Setup initial timer
void setup_timer(){
	my_timer = create_timer( 1000 );
}

//runs timer
void process_timer () {
  if (timer_init <= 0) {
    timer_init = get_current_time();
  }

  total_sec = (round( get_current_time() - timer_init ));
  second = total_sec % 60;
  minute = total_sec / 60;

}

//Setup ship
void setup_player(void) {
  //sprite middle  bottom
  int x = ( screen_width() - SHIP_WIDTH ) / 2 + 1;
  int y = screen_height() - SHIP_HEIGHT - 1;
  ship = sprite_create(x, y, SHIP_WIDTH, SHIP_HEIGHT, ship_image);
  sprite_draw(ship);
  sprite_move(ship, -1, 0);
}

//Setup diamonds
void setup_diamond(){
  //spawn diamond
  for ( int i = 0; i < MAX_DIAMOND; i++ ) {
    int randx = rand() % ( screen_width() ) - 1;

		diamond[i] = sprite_create( randx, 3, diamond_big_w, diamond_big_h, diamond_big_img );
		sprite_turn_to( diamond[i], 0.05, 0 );
		sprite_turn( diamond[i], (rand() % 120) + 210);
	}
}

//Setup missile
void setup_missile(){
  missile = sprite_create( round(sprite_x(ship)), screen_height() - 1, 1, 2, missile_img );
  sprite_turn_to( missile, 0.2, 0);
  sprite_turn( missile, 90);
  sprite_hide( missile );
}

//process_missile
void process_missile(){
  if ( sprite_visible( missile ) ){
    sprite_step( missile );
  }

  //if missile collide with top
  if (sprite_y(missile) < 3 ) {
    sprite_hide(missile);
  }

}
//Launch missile
void launch_missile(){
  if (sprite_visible( missile ) == false){
    //set new xy of missile
    missile->x = sprite_x(ship) + 3;
    missile->y = sprite_y(ship) - 2;
    //Launch missile
    sprite_show( missile );
  }
}
//process_diamond
void process_diamond(){
  for ( int i = 0; i < MAX_DIAMOND; i++ ) {
    int dx = round(sprite_x(diamond[i]));
    int dy = round(sprite_y(diamond[i]));

    double sdx = sprite_dx(diamond[i]);
    double sdy = sprite_dy(diamond[i]);

    //Border collision detection
    if ( dx <= 0 ) {
      sdx = fabs(sdx);
    }
    else if ( dx >= screen_width() - sprite_width(diamond[i]) ) {
      sdx = -fabs(sdx);
    }

    if ( dy <= 2 ) {
      sdy = fabs(sdy);
    }
    else if ( dy >= screen_height() - sprite_height(diamond[i])) {
      sdy = -fabs(sdy);
    }

    if ( sdx != sprite_dx(diamond[i]) || sdy != sprite_dy(diamond[i]) ) {
      sprite_back( diamond[i] );
      sprite_turn_to( diamond[i], sdx, sdy);
    }

    if ( sprite_visible( diamond[i] ) ) {
			sprite_step( diamond[i] );
		}
	}
}

//collision between s1 and s2
bool collision(sprite_id s1, sprite_id s2){
  int s1_top = round(sprite_y(s1));
  int s1_base = s1_top + sprite_height(s1) - 1;
  int s1_left = round(sprite_x(s1));
  int s1_right = s1_left + sprite_width(s1) - 1;

  int s2_top = round(sprite_y(s2));
  int s2_base = s2_top + sprite_height(s2) - 1;
  int s2_left = round(sprite_x(s2));
  int s2_right = s2_left + sprite_width(s2) - 1;

  if ( s1_base <= s2_top ) {
    return false;
  }
  else if ( s1_top > s2_base ) {
    return false;
  }
  else if ( s1_right < s2_left ) {
    return false;
  }
  else if ( s1_left > s2_right ) {
    return false;
  }
  else{
    return true;
  }
}

//Setup all
void setup_all(){
  setup_screen();
  setup_player();
  setup_diamond();
  setup_timer();
  setup_missile();
}

//respawn
void respawn (){
  clear_screen();
  sprite_destroy(ship);
  sprite_destroy(missile);
  for (int i = 0; i < MAX_DIAMOND; i++){
    sprite_destroy(diamond[i]);
  }

  setup_screen();
  setup_player();
  setup_diamond();
  setup_missile();

  show_screen();
}

//Key press
void key_press(int key){

  if ( game_pause == true && key >= 0) {
    game_pause = false;
    while ( get_char() >= 0 ) {}
  }

  //move left right
  else if ( ( key == 'a' || key == 'A' ) && round(sprite_x(ship)) > 1 ) {
    sprite_move(ship, -1, 0);
  }
  else if ( ( key == 'd' || key == 'D' ) && round(sprite_x(ship)) < screen_width() - sprite_width(ship) - 1 ) {
    sprite_move(ship, +1, 0);
  }
  //space to launch_missile
  else if ( key == ' '){
    launch_missile();
  }
  //q exit
  else if (key == 'q' || key == 'Q'){
  exit(0);
  }
  //h for help/pause screen
  else if (key == 'h' || key == 'H') {
    game_pause = true;
  }
}

//Key press while game over
void game_over_key(){
  int key = get_char();

  if ( key == 'y' || key == 'Y' ){
    lives = 10;
    score = 0;
    diamond_count = MAX_DIAMOND;
    timer_init = get_current_time();
    game_over = false;
    while ( get_char() >= 0 ) {}
  }
  else if ( key == 'n' || key == 'N' ){
    clear_screen();
    exit(0);
  }
}

//Main process
void process(void) {
  	int key = get_char();
    key_press(key);

    process_diamond();
    process_missile();
    process_timer();

		clear_screen();

		sprite_draw(ship);
    sprite_draw(missile);

    if ( lives == 0 ){
      game_over = true;
    }

    //draw diamond
    for ( int i = 0; i < MAX_DIAMOND; i++ ) {
      sprite_draw( diamond[i] );
      //collision check
      if ( collision(ship, diamond[i]) == true ) {
        sprite_hide(ship);
        sprite_hide(missile);
        lives--;
        diamond_count = MAX_DIAMOND;
        respawn();
      }
      else if ( collision( missile, diamond[i]) == true ) {
        sprite_hide( diamond[i] );
        sprite_hide( missile );
        //set missile out of game field.
        missile->x = -10;
        missile->y = -10;
        diamond[i]->x = screen_width();
        diamond[i]->y = 0;
        score++;

        //If all diamond are destroyed, spawn new
        diamond_count--;
        if (diamond_count == 0){
          for (int i = 0; i < MAX_DIAMOND; i++){
            sprite_destroy(diamond[i]);
          }
          setup_diamond();
          diamond_count = MAX_DIAMOND;
          show_screen();
        }
      }
    }
}

void run(){

  while ( !game_over && !game_pause) {
		process();
    draw_border();
    draw_status();

		if ( update_screen ) {
			show_screen();
		}
		timer_pause(DELAY);
	}

  while ( game_pause ){
    clear_screen();
    while ( get_char() >= 0 ) {}
    pause_screen();
    int key = get_char();
    key_press(key);
  }

  while ( game_over ){
    clear_screen();
    while ( get_char() >= 0 ) {}
    game_over_screen();
    game_over_key();
  }
  run();
}

int main(void) {
  setup_all();
  run();
	return 0;
}
