#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

//  (a) Declare four global integer variables, as follows:
//      left - the horizontal location of the left edge of the rectangle
//      top - the vertical location of the top edge of the rectangle
//      width - the width of the rectangle.
//      height - the height of the rectangle.
//	(b) Declare a global variable of type char called ch.
int left;
int top;
int width;
int height;
char ch;

void fill_rect(void) {
    //  (c) Test to see if either of width or height is less than 1.
    //      If so, the function should stop immediately and draw nothing.
    if (height < 1 || width < 1) {
      //do nothing
      return;
    }
    else{
    //  (d) Calculate the x-coordinate of the right-hand side of the rectangle.
    //      Store the value in an integer variable called rect_right.
    int rect_right = left + width - 1;
    //  (e) Calculate the y coordinate of the bottom edge of the rectangle.
    //      Store the result in an integer variable called rect_base.
    int rect_base = top + height - 1;
    //  (f.a) Set up for loop that uses an integer variable called y_ctr to
    //      iterate from the top of the rectangle to the bottom of the rectangle.
    //      Initially y_ctr should be equal to top.
    //      The loop body should run while y_ctr is less than or equal to rect_base.
    for ( int y_ctr = top; y_ctr <= rect_base; y_ctr++) {
      draw_line(left, y_ctr, rect_right, y_ctr, ch );
    }
        //  (g) Draw a horizontal line from left to rect_right, with
        //      y-coordinate equal to y_ctr, using the character specified by
        //      ch.
    //  (f.b) End the loop.
    }
}
int main(void) {
    setup_screen();
    fill_rect();
    show_screen();
    timer_pause(500000);
    cleanup_screen();
    return 0;
}
