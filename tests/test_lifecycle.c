/*
 * This tests
 * - initialiser methods
 * - destructor methods
 * - managing of operations on destroyed objects
 */

#include <stdio.h>

#include "bit_array.h"
#include "test_utils.h"

int main(void) {
  printf("== Lifecycle tests ==\n");

  /* init valid */
  bit_array bb = make_bit_array_or_die(10);

  /* init invalid sizes */
  {
    bit_array tmp;
    CHECK("init size 0 fails", init_bit_array(0, &tmp) == 1);
    CHECK("init negative size fails", init_bit_array(-5, &tmp) == 1);
  }

  /* destroy resets fields */
  destroy_and_check(&bb);

  /* operations on destroyed object should fail / be invalid */
  CHECK("set_bit_busy on destroyed fails", set_bit_busy(0, &bb) == 1);
  CHECK("set_range_busy on destroyed fails", set_range_busy(0, 0, &bb) == 1);

  CHECK("is_bit_available on destroyed invalid",
        is_bit_available(0, &bb) == -1);
  CHECK("is_range_available on destroyed invalid",
        is_range_available(0, 0, &bb) == -1);

  printf("Failures: %d\n", g_failures);
  return g_failures ? 1 : 0;
}
