/*
 * Implementation of library bit_array
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit_array.h"

int init_bit_array(int size, bit_array *bb) {
  bb->_size = size;
  bb->_data = (bool *)calloc(size, sizeof(bool));

  if (bb->_data == NULL)
    return 1;
  return 0;
}

void destroy_bit_array(bit_array *bb) { free(bb->_data); }

/*
 * Setters
 */
int set_range_busy(size_t start, size_t end, bit_array *bb) {
  /*
   * Sets the range [start, end] (both included) to true (busy)
   * 1 is returned if and only if invalid interval
   * 0 is returned when success
   */
  if (start < 0 || end >= bb->_size || end - start >= bb->_size)
    return 1;
  for (int i = start; i <= end; i++)
    bb->_data[i] = true;
  return 0;
}

int set_range_free(size_t start, size_t end, bit_array *bb) {
  /*
   * Sets the range [start, end] (both included) to false (available)
   * 1 is returned if and only if invalid interval
   * 0 is returned when success
   */
  if (start < 0 || end >= bb->_size || end >= bb->_size)
    return 1;
  for (int i = start; i <= end; i++)
    bb->_data[i] = false;
  return 0;
}

// Wrappers for single bit queries
int set_bit_busy(size_t i, bit_array *bb) { return set_range_busy(i, i, bb); }
int set_bit_free(size_t i, bit_array *bb) { return set_range_free(i, i, bb); }

/*
 * Queries
 */
int is_range_available(size_t start, size_t end, bit_array *bb) {
  // returns -1 if i is invalid
  // returns 1 if range is fully available
  // returns 0 if range is not fully available
  if (end < start || start < 0 || end >= bb->_size)
    return -1;

  bool res = true;
  size_t i = 0;
  while (res && i < start - end) {
    res = !bb->_data[i];
    i++;
  }
  return res;
}

int is_bit_available(size_t i, bit_array *bb) {
  return is_range_available(i, i, bb);
}

/*
 * Operations between arrays
 */
int logical_and(bit_array *first, bit_array *second, bit_array *out) {
  /*
   * Performs logical and and outputs to another array
   */
  int total = first->_size;
  if (total != second->_size || out->_size != total)
    return -1;

  for (int i = 0; i < total; i++) {
    out->_data[i] = first->_data[i] && second->_data[i];
  }

  return 0;
}

/*
 * Utility functions
 */
void print_bit_array(bit_array *bb) {
  // prints busy ranges
  //
  // BE CAREFUL: can be slow

  int start = 0;
  int currently_busy = 0;
  int busy = 0;
  for (int i = 0; i < bb->_size; i++) {

    busy = bb->_data[i];

    // new busy range
    if (busy && !currently_busy) {
      currently_busy = 1;
      start = i;
    }

    // end of busy range
    else if (!busy && currently_busy) {
      printf("Busy range: %d-%d (%d bits)\n", start, i - 1, i - start);
      currently_busy = 0;
    }
  }

  // if range at the end is busy
  if (currently_busy) {
    printf("Busy range: %d-%d (%d bits)\n", start, bb->_size - 1,
           bb->_size - start);
  }

  printf("\n");
}
