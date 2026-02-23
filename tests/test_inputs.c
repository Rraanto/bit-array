/*
 * This tests
 * - if invalid inputs are handled correctly
 *   for all functions requiring:
 *   (start-end) range inputs
 */

#include <stddef.h>
#include <stdio.h>

#include "bit_array.h"
#include "test_utils.h"

int main(void) {
  printf("== Invalid input tests ==\n");

  const int size_int = 10;
  const size_t n = (size_t)size_int;
  bit_array bb = make_bit_array_or_die(size_int);

  /* Invalid ranges */
  CHECK("set_range_busy start>end fails", set_range_busy(5, 4, &bb) == 1);
  CHECK("set_range_free start>end fails", set_range_free(5, 4, &bb) == 1);

  CHECK("set_range_busy end==size fails", set_range_busy(0, n, &bb) == 1);
  CHECK("set_range_free end==size fails", set_range_free(0, n, &bb) == 1);

  CHECK("is_range_available end==size invalid",
        is_range_available(0, n, &bb) == -1);
  CHECK("_any_in_range end==size invalid", _any_in_range(0, n, &bb) == -1);

  CHECK("set_bit_busy idx==size fails", set_bit_busy(n, &bb) == 1);
  CHECK("is_bit_available idx==size invalid", is_bit_available(n, &bb) == -1);

  /* "Negative" indices cannot be passed directly (size_t), so test via cast. */
  size_t neg = (size_t)-1;
  CHECK("set_bit_busy cast(-1) fails", set_bit_busy(neg, &bb) == 1);
  CHECK("is_bit_available cast(-1) invalid", is_bit_available(neg, &bb) == -1);

  /* NULL pointer handling */
  CHECK("set_bit_busy NULL bb fails", set_bit_busy(0, NULL) == 1);
  CHECK("is_bit_available NULL bb invalid", is_bit_available(0, NULL) == -1);
  CHECK("_any_in_range NULL bb invalid", _any_in_range(0, 0, NULL) == -1);

  destroy_and_check(&bb);

  printf("Failures: %d\n", g_failures);
  return g_failures ? 1 : 0;
}
