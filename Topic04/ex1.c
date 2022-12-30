#include <stdlib.h>
#include <cab202_graphics.h>

//  (a) Begin the definition a function called fill_rect that returns nothing,
//      but requires the following arguments:
//      leftmost - an integer that specifies the absolute horizontal location
//          of the left hand side of the rectangle.
//      upper - an integer that specifies the absolute vertical location
//          of the upper edge of the rectangle.
//      rightmost - an integer that specifies the absolute horizontal location
//          of the right hand side of the rectangle.
//      lower - an integer that specifies the absolute vertical location
//          of the lower edge of the rectangle.
//      display_char = a character value that will be used to draw the
//          display the rectangle.

    //  (c) Test to see if either the width or height of the rectangle is less
    //      than 1. If so, the function should stop immediately and draw nothing.

    //  (d) Use statements of your own choice to fill the region defined by the
    //      first four arguments with the designated character value.

//  (b) End the definition of fill_rect.
void fill_rect( int leftmost, int upper, int rightmost, int lower, char display_char ){

  if ( (rightmost + leftmost) < 1 || (lower + upper) < 1 || 1 > rightmost || upper > lower ) {

    if ( rightmost == 1 ) {
      draw_line(leftmost, upper, leftmost, lower, display_char);
    }
    else if ( lower == upper) {
      draw_line(leftmost, upper, rightmost, lower, display_char);
    }
    else{
      for( int i = upper; i <= lower; i++){
        draw_line(leftmost, i, rightmost, i, display_char);
      }
    }
  }
}

void hrule( void );
void vrule( void );

int main( void ) {
    setup_screen();

    int leftmost = 30;
    int upper = 11;
    int rightmost = 68;
    int lower = 18;
    int display_char = '@';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();




    /*
    // draw a box.
    int leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    int upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    int rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    int lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    int display_char = '@';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = '&';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box with zero width.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost - 1;
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = '*';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = '#';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box with negative width.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost - 2;
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = '!';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = '+';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box with zero height.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper - 1;
    display_char = 'a';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = 'b';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box with negative right.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper - 2;
    display_char = 'c';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();

    // draw a box.
    leftmost = 2 + rand() % ( screen_width() - 2 ) / 2;
    upper = 2 + rand() % ( screen_height() - 2 ) / 2;
    rightmost = leftmost + 1 + rand() % ( screen_width() - leftmost - 1 );
    lower = upper + 1 + rand() % ( screen_height() - upper - 1 );
    display_char = 'd';
    clear_screen();
    hrule();
    vrule();
    draw_formatted( 5, 1,
        "left: %4d, upper: %4d, right: %4d, lower: %4d - Press key to continue...",
        leftmost, upper, rightmost, lower
        );
    fill_rect( leftmost, upper, rightmost, lower, display_char );
    show_screen();
    wait_char();
    */

    cleanup_screen();
    return 0;
}

void hrule( void ) {
    for ( int x = 0; x < screen_width(); x++ ) {
        draw_char( x, 0, '0' + ( x % 10 ) );
    }
}

void vrule( void ) {
    for ( int y = 0; y < screen_height(); y++ ) {
        draw_char( 0, y, '0' + ( y % 10 ) );
    }
}
