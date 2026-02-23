#include <stdbool.h>
#include <stdio.h>

#include "bit_array.h"

int main() {

  int size = 10;

  bit_array bb;
  int init_success = init_bit_array(size, &bb) == 0;
  if (!init_success)
    printf("Bit array initialisation failed\n");

  set_bit_busy(2, &bb);
  set_range_busy(1, 3, &bb);

  printf("The size of the bit_array is: %d \n", bb._size);
  print_bit_array(&bb); // 0-1-1-1-0...

  set_range_free(2, 9, &bb);
  print_bit_array(&bb); // 0-1-0...

  set_range_busy(4, 8, &bb);
  print_bit_array(&bb); // 0-1-0-0-1-1-1-1-1-0

  // testing invalid operations
  int success;
  success = !set_bit_busy(-1, &bb) == -1;
  if (!success)
    printf("query for bit %d failed\n", -1);

  success = !is_bit_available(-1, &bb) == -1;
  if (!success)
    printf("query for bit %d failed\n", -1);

  success = !is_range_available(-2, 3, &bb) == -1;
  if (!success)
    printf("query for range %d %d failed \n", -2, 3);

  success = !is_range_available(4, 6, &bb) == -1;

  if (!success)
    printf("query for range %d, %d failed\n", 4, 6);

  printf("\n");

  print_bit_array(&bb);

  printf("Checking if any_in_range is the same as !is_range_available for "
         "every valid range\n");

  for (int start = 0; start < size - 1; start++) {
    for (int end = start + 1; end < size; end++) {
      printf("range %d-%d: ", start, end);
      if (_any_in_range(start, end, &bb) !=
          !is_range_available(start, end, &bb))
        printf("Error: different query results for _any_in_range and "
               "is_range_available\n",
               start, end);
      else
        printf("Ok\n");
    }
  }

  destroy_bit_array(&bb);
  return 0;
}
