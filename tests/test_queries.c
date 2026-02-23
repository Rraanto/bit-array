/*
 * This tests:
 * - per-bit query
 * - per-range query for:
 *   fully busy range
 *   fully free range
 *   partially busy range
 **/

#include <stddef.h>
#include <stdio.h>

#include "bit_array.h"
#include "test_utils.h"

int main(void) {
  printf("== Query tests ==\n");

  const int size_int = 10;
  const size_t n = (size_t)size_int;
  bit_array bb = make_bit_array_or_die(size_int);

  /*
   * Build a known pattern (busy bits):
   * [1,1], [3,4], [7,9]
   */
  CHECK("set_bit_busy(1)", set_bit_busy(1, &bb) == 0);
  CHECK("set_range_busy(3,4)", set_range_busy(3, 4, &bb) == 0);
  CHECK("set_range_busy(7,9)", set_range_busy(7, 9, &bb) == 0);

  /* Per-bit queries */
  check_bit_available(&bb, 0, 1);
  check_bit_available(&bb, 1, 0);
  check_bit_available(&bb, 2, 1);
  check_bit_available(&bb, 3, 0);
  check_bit_available(&bb, 4, 0);
  check_bit_available(&bb, 5, 1);
  check_bit_available(&bb, 6, 1);
  check_bit_available(&bb, 7, 0);
  check_bit_available(&bb, 9, 0);

  /* Range queries */
  check_range_available(&bb, 0, 0, 1); /* free */
  check_range_available(&bb, 1, 1, 0); /* busy */
  check_range_available(&bb, 3, 4, 0); /* busy */
  check_range_available(&bb, 5, 6, 1); /* free */
  check_range_available(&bb, 2, 5, 0); /* partially busy */

  /* Primitive equivalence on all valid ranges (includes [i,i]) */
  check_any_matches_not_available(&bb, n);

  destroy_and_check(&bb);

  printf("Failures: %d\n", g_failures);
  return g_failures ? 1 : 0;
}
