#include <stdio.h>

void main( void ) {
  printf("Listing integers in reverse, from 399 down to but not including 99 in steps of 6.\n" );

  int start = 399;
  int end = 99;


  while (start >= end) {
    if (start != end) {
      printf("%d\n", start);
    }
    else{

    }

    start = start-6;
  }
}
