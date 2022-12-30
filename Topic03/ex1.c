#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>

//  (a) Declare four global integer variables, as follows:
//      x - the horizontal location of the left edge of the rectangle
//      y - the vertical location of the top edge of the rectangle
//      w - the width of the rectangle.
//      h - the height of the rectangle.
int x = 0;
int y = 0;
int w = 0;
int h = 0;

//  (b) Declare a global variable of type char called c.
//      This is the character that is to be used to render the rectangle.
char c;

void draw_rect(void) {
    //  (c) Insert code to draw the outline of the rectangle defined by the global variables.
    //      If either of the width or height is less than or equal to zero,
    //      the function must not draw anything.

    if (w <= 0 || h <= 0) {
    }
    //width 1
    else if (w == 1) {
      draw_line( x, y, x, (h + y - 1), c );
      draw_line( x, (h + y - 1), x, (h + y - 1), c );
    }
    //height 1
    else if (h == 1) {
      draw_line( (w + x - 1), y, x, y, c );
      draw_line( x, y, (w + x - 1), y, c );
    }
    //standard
    else{
      draw_line( (w + x - 1), y, x, y, c );
      draw_line( x, y, x, (h + y - 1), c );
      draw_line( x, (h + y - 1), (w + x - 1), (h + y - 1), c );
      draw_line( (w + x - 1), (h + y - 1), (w + x - 1), y, c );
    }
}


int main(void) {
    setup_screen();
    x = 22;
    y = 0;
    w = 0;
    h = 5;
    c = '@';
    draw_rect();
    show_screen();

/*
    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = '@';
    draw_rect();
    show_screen();

    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = '&';
    draw_rect();
    show_screen();

    // draw a box with zero width.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 0;
    h = 1 + rand() % (screen_height() - y - 1);
    c = '*';
    draw_rect();
    show_screen();

    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = '#';
    draw_rect();
    show_screen();

    // draw a box with negative width.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = -rand() % screen_width();
    h = 1 + rand() % (screen_height() - y - 1);
    c = '!';
    draw_rect();
    show_screen();

    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = '+';
    draw_rect();
    show_screen();

    // draw a box with zero height.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 0;
    c = 'a';
    draw_rect();
    show_screen();

    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = 'b';
    draw_rect();
    show_screen();

    // draw a box with negative width.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = -rand() % screen_width();
    h = 1 + rand() % (screen_height() - y - 1);
    c = 'c';
    draw_rect();
    show_screen();

    // draw a box.
    x = rand() % screen_width() / 2;
    y = rand() % screen_height() / 2;
    w = 1 + rand() % (screen_width() - x - 1);
    h = 1 + rand() % (screen_height() - y - 1);
    c = 'd';
    draw_rect();
    show_screen();
*/
    timer_pause(500000);
    cleanup_screen();
    return 0;
}
