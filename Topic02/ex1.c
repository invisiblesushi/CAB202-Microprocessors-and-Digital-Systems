#include <stdio.h>

void main( void ) {
	// (a) Print the introductory message.
  printf("Integers less than or equal to 352, and greater than 14...\n" );
	// (b) Declare an integer variable that will be used as a counter.
	//     Make it initially equal to the start value.
  int start = 14;
  int end = 352;

	// (c) Begin a WHILE statement that will execute its loop body if the
	//     counter is less than or equal to the end value.
  while (start <= end) {
		// (e) Print the current value of the counter variable.
    printf("%d\n", start);
    // (f) Add 1 to the counter variable.
    start++;
  }
	// (d) End the WHILE statement that starts at (c).
}
