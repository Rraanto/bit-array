/*
 * This tests bitwise operations:
 * - two identical arrays
 * - arrays with disjoint busy ranges
 * - arrays with overlapping ranges
 */
#include <stddef.h>
#include <stdio.h>

#include "bit_array.h"
#include "test_utils.h"

int main(void) {
  printf("== Operations tests (logical_and) ==\n");

  const int size_int = 10;
  const size_t n = (size_t)size_int;

  bit_array a = make_bit_array_or_die(size_int);
  bit_array b = make_bit_array_or_die(size_int);
  bit_array out = make_bit_array_or_die(size_int);

  /* Case 1: identical arrays -> out equals input */
  CHECK("a busy [2,6]", set_range_busy(2, 6, &a) == 0);
  CHECK("b busy [2,6]", set_range_busy(2, 6, &b) == 0);
  CHECK("logical_and identical succeeds", logical_and(&a, &b, &out) == 0);

  for (size_t i = 0; i < n; i++) {
    int expected = is_bit_available(i, &a);
    check_bit_available(&out, i, expected);
  }

  /* Case 2: disjoint busy ranges -> out all free */
  clear_all(&a);
  clear_all(&b);
  clear_all(&out);

  CHECK("a busy [0,2]", set_range_busy(0, 2, &a) == 0);
  CHECK("b busy [5,7]", set_range_busy(5, 7, &b) == 0);
  CHECK("logical_and disjoint succeeds", logical_and(&a, &b, &out) == 0);
  check_range_available(&out, 0, n - 1, 1);

  /* Case 3: overlap -> out contains overlap only */
  clear_all(&a);
  clear_all(&b);
  clear_all(&out);

  CHECK("a busy [1,4]", set_range_busy(1, 4, &a) == 0);
  CHECK("b busy [3,6]", set_range_busy(3, 6, &b) == 0);
  CHECK("logical_and overlap succeeds", logical_and(&a, &b, &out) == 0);

  /* Overlap is [3,4]. */
  check_range_available(&out, 3, 4, 0);
  check_range_available(&out, 1, 2, 1);
  check_range_available(&out, 5, 6, 1);

  /* Size mismatch should fail */
  bit_array small = make_bit_array_or_die(5);
  CHECK("logical_and size mismatch fails", logical_and(&a, &small, &out) == 1);
  destroy_and_check(&small);

  destroy_and_check(&a);
  destroy_and_check(&b);
  destroy_and_check(&out);

  printf("Failures: %d\n", g_failures);
  return g_failures ? 1 : 0;
}
