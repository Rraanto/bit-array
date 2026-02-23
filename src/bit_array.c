/*
 * Implementation of library bit_array
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit_array.h"

int init_bit_array(int size, bit_array *bb) {
  if (bb == NULL)
    return 1;

  // initialises the size to 0 (without data)
  if (size <= 0) {
    bb->_size = 0;
    bb->_data = NULL;
    return 1; // empty
  }

  // allocates memory of size <size> with values zero (false)
  bb->_size = size;
  bb->_data = (bool *)calloc(size, sizeof(bool));

  if (bb->_data == NULL)
    return 1;
  return 0;
}

void destroy_bit_array(bit_array *bb) {
  if (bb == NULL)
    return;

  // frees the memory allocated for the bit_array data
  free(bb->_data);
  bb->_data = NULL;
  bb->_size = 0;
}

/*
 * Setters
 */
int set_range_busy(size_t start, size_t end, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return 1;
  if (start > end)
    return 1;
  if (end >= bb->_size)
    return 1;

  // sets all the bits start, ... end to 1 (true)
  if (start < 0 || end >= bb->_size || end - start >= bb->_size)
    return 1;
  for (int i = start; i <= end; i++)
    bb->_data[i] = true;
  return 0;
}

int set_range_free(size_t start, size_t end, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return 1;
  if (start > end)
    return 1;
  if (end >= bb->_size)
    return 1;

  // sets all the bits start, ... end to false
  for (size_t i = start; i <= end; i++) {
    bb->_data[i] = false;
  }
  return 0;
}

// Wrappers for single bit queries
int set_bit_busy(size_t i, bit_array *bb) {
  if (bb == NULL)
    return 1;
  return set_range_busy(i, i, bb);
}
int set_bit_free(size_t i, bit_array *bb) {
  if (bb == NULL)
    return 1;
  return set_range_free(i, i, bb);
}

/*
 * Queries
 */
int _any_in_range(size_t start, size_t end, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (start > end)
    return -1;
  if (end >= bb->_size)
    return -1;

  // checks for any busy bits in the range
  for (size_t i = start; i <= end; i++) {
    if (bb->_data[i])
      return 1;
  }

  return 0;
}

int is_range_available(size_t start, size_t end, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (start > end)
    return -1;
  if (end >= bb->_size)
    return -1;

  // use _any_in_range to query for freeness
  return (_any_in_range(start, end, bb) == 0);
}

int is_bit_available(size_t i, bit_array *bb) {
  if (bb == NULL)
    return -1;
  if (i < 0 || i >= bb->_size)
    return -1;

  // use is_range_available to query if bit i is true
  return is_range_available(i, i, bb);
}

/*
 * Operations between arrays
 */
int logical_and(bit_array *first, bit_array *second, bit_array *out) {
  if (first == NULL || second == NULL || out == NULL)
    return 1;
  if (first->_data == NULL || second->_data == NULL || out->_data == NULL)
    return 1;
  if (first->_size != second->_size || out->_size != first->_size)
    return 1;

  // iterate on indexes and compute logical and per bit
  for (size_t i = 0; i < first->_size; i++) {
    out->_data[i] = first->_data[i] && second->_data[i];
  }
  return 0;
}

/*
 * Utility functions
 *
 * Does nothing if bit array pointer is NULL
 */
void print_bit_array(bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return;
  // prints busy ranges
  // BE CAREFUL: can be slow

  size_t start = 0;
  int currently_busy = 0;
  for (size_t i = 0; i < bb->_size; i++) {
    bool busy = bb->_data[i];
    busy = bb->_data[i];

    // new busy range
    if (busy && !currently_busy) {
      currently_busy = 1;
      start = i;
    }

    // end of current busy range
    else if (!busy && currently_busy) {
      printf("Busy range: %zu-%zu (%zu bits)\n", start, i - 1, i - start);
      currently_busy = 0;
    }
  }

  // if range at the end is busy
  if (currently_busy) {
    printf("Busy range: %zu-%zu (%zu bits)\n", start, bb->_size - 1,
           bb->_size - start);
  }
}
