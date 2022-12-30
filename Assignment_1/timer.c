#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

bool game_over = false;
timer_id my_timer;
double init = 0;

void setup_timer() {
	my_timer = create_timer( 1000 );
}

void process_timer () {
  if (init <= 0) {
    init = get_current_time();
  }

  int sec = round( get_current_time() - init );

	if ( timer_expired( my_timer ) ) {
		printf( "\n sec %d  curr %f", sec, get_current_time() );
		fflush(stdout);
	}
}

int main() {
	setup_timer();

	while ( ! game_over ) {
		process_timer();
		timer_pause( 100 );
	}

	cleanup_screen();
}
