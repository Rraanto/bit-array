#include "bit_array.h"
#include <stdbool.h>

#include "bit_array.h"
/*
 * This tests the logical and on two simple
 * periodic tables
 */

#define TOTAL 40

int main() {
  bit_array first;
  bit_array second;
  bit_array intersection;

  init_bit_array(TOTAL, &first);
  init_bit_array(TOTAL, &second);
  init_bit_array(TOTAL, &intersection);

  int a, b, c, d;
  for (int i = 0; i < 4; i++) {
    // busy 1 : [4, 8]
    a = 10 * i + 4;
    b = 10 * i + 8;

    // busy 2 : [2, 6]
    c = 10 * i + 2;
    d = 10 * i + 6;

    set_range_busy(a, b, &first);
    set_range_busy(c, d, &second);
  }

  logical_and(&first, &second, &intersection);

  printf("First array: \n");
  print_bit_array(&first);
  printf("\n");

  printf("Second array: \n");
  print_bit_array(&second);
  printf("\n");

  printf("Intersection: \n");
  print_bit_array(&intersection);
  printf("\n");

  destroy_bit_array(&first);
  destroy_bit_array(&second);
  destroy_bit_array(&intersection);
}
