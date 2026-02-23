#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>
#include <stdio.h>

#include "bit_array.h"

/*
 * Minimal test helpers (no external framework).
 * Each test file will include this header and use the CHECK macro.
 */

static int g_failures = 0;

// marco to generate check function
#define CHECK(MSG, COND)                                                       \
  do {                                                                         \
    if (!(COND)) {                                                             \
      g_failures++;                                                            \
      printf("[FAIL] %s\n", (MSG));                                            \
    } else {                                                                   \
      printf("[ OK ] %s\n", (MSG));                                            \
    }                                                                          \
  } while (0)

/* Basic check helpers */
static void check_bit_available(const bit_array *bb, size_t idx, int expected) {
  int got = is_bit_available(idx, bb);
  char msg[128];
  snprintf(msg, sizeof(msg), "bit %zu available == %d", idx, expected);
  CHECK(msg, got == expected);
}

static void check_range_available(const bit_array *bb, size_t start, size_t end,
                                  int expected) {
  int got = is_range_available(start, end, bb);
  char msg[160];
  snprintf(msg, sizeof(msg), "range %zu-%zu available == %d", start, end,
           expected);
  CHECK(msg, got == expected);
}

/*
 * Checks that the primitive any-busy query equals !availability
 * for all valid inclusive ranges [start,end].
 */
static void check_any_matches_not_available(const bit_array *bb, size_t nbits) {
  for (size_t start = 0; start < nbits; start++) {
    for (size_t end = start; end < nbits; end++) {
      int any = _any_in_range(start, end, bb);
      int avail = is_range_available(start, end, bb);

      char msg[220];
      snprintf(msg, sizeof(msg),
               "_any_in_range(%zu,%zu) == !is_range_available(%zu,%zu)", start,
               end, start, end);
      CHECK(msg, any == !avail);
    }
  }
}

/* Create + validate */
static bit_array make_bit_array_or_die(int size_int) {
  bit_array bb;
  int rc = init_bit_array(size_int, &bb);
  CHECK("init_bit_array returned success", rc == 0);
  if (rc != 0) {
    bb._size = 0;
    bb._data = NULL;
    return bb;
  }
  CHECK("bit_array _data non-NULL after init", bb._data != NULL);
  CHECK("bit_array _size matches requested size", bb._size == (size_t)size_int);
  return bb;
}

static void destroy_and_check(bit_array *bb) {
  destroy_bit_array(bb);
  CHECK("destroy_bit_array sets _data to NULL", bb->_data == NULL);
  CHECK("destroy_bit_array sets _size to 0", bb->_size == 0);
}

/* Simple helper used in operation tests */
static void clear_all(bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return;
  if (bb->_size == 0)
    return;
  (void)set_range_free(0, bb->_size - 1, bb);
}

#endif /* TEST_UTILS_H */
