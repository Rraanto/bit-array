/*
 * This tests:
 * - setting busy bits at indexes (boundaries)
 * - setting busy bits in ranges touching the boundaries
 * - setting and freeing overlapping ranges
 */

#include <stddef.h>
#include <stdio.h>

#include "bit_array.h"
#include "test_utils.h"

int main(void) {
  printf("== Setter tests ==\n");

  const int size_int = 10;
  const size_t n = (size_t)size_int;
  bit_array bb = make_bit_array_or_die(size_int);

  /* Initially all free */
  for (size_t i = 0; i < n; i++) {
    check_bit_available(&bb, i, 1);
  }

  /* Boundary single bits */
  CHECK("set_bit_busy(0)", set_bit_busy(0, &bb) == 0);
  CHECK("set_bit_busy(last)", set_bit_busy(n - 1, &bb) == 0);
  check_bit_available(&bb, 0, 0);
  check_bit_available(&bb, n - 1, 0);

  /* Ranges touching boundaries */
  CHECK("set_range_busy(0,3)", set_range_busy(0, 3, &bb) == 0);
  for (size_t i = 0; i <= 3; i++) {
    check_bit_available(&bb, i, 0);
  }

  CHECK("set_range_free(0,3)", set_range_free(0, 3, &bb) == 0);
  for (size_t i = 0; i <= 3; i++) {
    check_bit_available(&bb, i, 1);
  }

  /* Overlapping ranges: busy then free inside overlap */
  CHECK("set_range_busy(2,6)", set_range_busy(2, 6, &bb) == 0);
  CHECK("set_range_busy(5,8)", set_range_busy(5, 8, &bb) == 0);
  check_range_available(&bb, 2, 8, 0);

  CHECK("set_range_free(4,7)", set_range_free(4, 7, &bb) == 0);

  /* Bits 2-3 should remain busy; 4-7 freed; 8 still busy */
  check_bit_available(&bb, 2, 0);
  check_bit_available(&bb, 3, 0);
  check_bit_available(&bb, 4, 1);
  check_bit_available(&bb, 7, 1);
  check_bit_available(&bb, 8, 0);

  destroy_and_check(&bb);

  printf("Failures: %d\n", g_failures);
  return g_failures ? 1 : 0;
}
