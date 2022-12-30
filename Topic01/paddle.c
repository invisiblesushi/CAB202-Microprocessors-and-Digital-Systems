#include "cab202_graphics.h"

void draw_paddles( void ) {
	// INSERT CODE HERE
  int sh = screen_height();
  int sw = screen_width();

  draw_line( 3, ((sh/2)-4)+3, 3, ((sh/2)+4)+3, '#');

  draw_line( (sw-2), ((sh/2)-4)+3, (sw-2), ((sh/2)+4)+3, '#');

	show_screen( );
}

int main( void ) {
	setup_screen();

	draw_paddles();

	draw_string( 0, screen_height( ) - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	return 0;
}
