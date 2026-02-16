/*
 * Declaration of library bit_array
 *
 * Defines a dynamic array of bits with
 * per-bit or per-range availability and comparison queries
 */

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bit_array {
  /*
   * Size-safe wrapper to the bit array
   */
  int _size;
  bool *_data; // true for busy bits, false for available
} bit_array;

/*
 * Constructor and destructor methods
 */
int init_bit_array(int size, bit_array *bb); // 1 is returned iff memory
                                             // allocation failed otherwise 0
void destroy_bit_array(bit_array *bb);

/*
 * setter methods
 *
 * for all of the following setter methods:
 *   1 is returned if wrong index given
 *   0 if success
 */
int set_range_busy(size_t start, size_t end, bit_array *bb);
int set_range_free(size_t start, size_t end, bit_array *bb);
int set_bit_busy(size_t bit, bit_array *bb);
int set_bit_free(size_t bit, bit_array *bb);

/*
 * Availability queries
 * For all of the following querying methods
 *
 * -1 is returned if index is wrong
 *  0 is returned if bit or range is NOT available
 *  1 is returned if bit or range is available
 */

int is_range_available(size_t start, size_t end, bit_array *bb);
int is_bit_available(size_t i, bit_array *bb);

/*
 * Operations between arrays
 */
// perform logical and on array
int logical_and(bit_array *first, bit_array *second, bit_array *out);

/*
 * input-output-utility function
 */
void print_bit_array(bit_array *bb);
/*
 * outputs all the busy ranges
 *
 * Be careful: can be slow
 */

#endif
