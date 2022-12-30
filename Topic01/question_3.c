/*	CAB202: Tutorial 1
*	Question 3 - Template
*
*	B.Talbot, February 2016
*	Queensland University of Technology
*/
#include "cab202_graphics.h"

int main() {
    // Setup the screen and display a waiting message
    setup_screen();
    draw_string(27, 11, "Waiting for key press...");
    show_screen();

    // Wait for the key press
    int key_in = wait_char();

    // Clear the screen
    clear_screen();

    // Draw a word with the character if supported, otherwise print a
    // string saying that the character is not supported.
    // TODO

    // Wait here forever
    while(1);

    // Clean up the screen and return (we will NEVER make it here!)
    cleanup_screen();
    return 0;
}
