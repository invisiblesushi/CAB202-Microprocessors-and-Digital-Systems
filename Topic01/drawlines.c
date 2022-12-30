#include "cab202_graphics.h"

void draw_lines(void){
	// (a) Draw a line from (18,5) to (74,23).
  draw_line( 18, 5, 74, 23, '@');
	// (b) Draw a line from (74,23) to (40,1).
  draw_line( 74, 23, 40, 1, '@');
	// (c) Draw a line from (40,1) to (16,21).
  draw_line( 40, 1, 16, 21, '@');
	// (d) Draw a line from (16,21) to (18,5).
  draw_line( 16, 21, 18, 5, '@');
	// Leave the following instruction unchanged.
	show_screen();
}

int main( void ) {
	setup_screen( );

	draw_lines();

	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char( );
	cleanup_screen( );
	return 0;
}
