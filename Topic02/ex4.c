#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// Configuration
#define DELAY (10) /* Millisecond delay between game updates */

#define ZOMBIE_WIDTH (5)
#define ZOMBIE_HEIGHT (5)

// Game state.
bool game_over = false; /* Set this to true when game is over */
bool update_screen = true; /* Set to false to prevent screen update. */

char * zombie_image =
/**/	"ZZZZZ"
/**/	"   Z "
/**/	"  Z  "
/**/	" Z   "
/**/	"ZZZZZ";

//	(a) Declare a sprite_id called zombie
	sprite_id zombie;
// Setup game.
void setup(void) {
    //  (b) Assign a value to zombie by creating a sprite at integer
    //      coordinates (x,y). Use integer arithmetic to calculate
    //      the location relative to the screen dimensions:
    //      x = 80% of available horizontal space on the screen.
    //      y = 10% of available vertical space on the screen.
    //      Use the formula set out in the statement of the problem.
		int zw = ZOMBIE_WIDTH;
		int zh = ZOMBIE_HEIGHT;
		int x = (50% - zw) / 2;
    int y = (50% - zh) / 2;
		zombie = sprite_create(x, y, zw, zh, zombie_image);
    //	(c) Draw the zombie.
		sprite_draw(zombie);
}

// Play one turn of game.
void process(void) {
			int w = screen_width();
      int h = screen_height();
    //	(d) Get a character code from standard input without waiting.
			int key = get_char();
    //  (e) Get the current screen coordinates of the zombie in integer variables
    //      by rounding the actual coordinates. Store the coordinates in a pair of
    //      variables called x and y, both of type int.
			int zx = round(sprite_x(zombie));
			int zy = round(sprite_y(zombie));

    //	(f) Move zombie left, ensuring that it always remains within the screen.
			if ( key == '4' && zx > 1 ) sprite_move(zombie, -1, 0);
    //	(g) Move zombie right, ensuring that it always remains within the screen.
			if ( key == '6' && zx < w - sprite_width(zombie) - 1 ) sprite_move(zombie, +1, 0);
    //	(h) Move zombie up, ensuring that it always remains within the screen.
			if ( key == '8' && zy > 1 ) sprite_move(zombie, 0, -1);
    //	(i) Move zombie down according to specification.
			if ( key == '2' && zy < h - sprite_height(zombie) - 1 ) sprite_move(zombie, 0, +1);
    //	(j) If the key is 'q', set game_over to true.
			if (key == 'q'){
			game_over = true;
      clear_screen();
			}
    // Leave next line intact.
		clear_screen();
    //	(k)	Draw the zombie.
		sprite_draw(zombie);
}

// Program entry point.
int main(void) {
	setup_screen();
	setup();
	show_screen();

	while ( !game_over ) {
		process();

		if ( update_screen ) {
			show_screen();
		}

		timer_pause(DELAY);
	}

	return 0;
}
