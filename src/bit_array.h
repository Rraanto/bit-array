/*
 * Declaration of library bit_array
 *
 * Defines a dynamic array of bits with
 * per-bit or per-range availability and comparison queries
 */

#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdbool.h>
#include <stddef.h> // size_t and other frequently used types
#include <stdio.h>
#include <stdlib.h>

typedef struct bit_array {
  /*
   * Size-safe wrapper to the bit array
   */
  size_t _size;
  bool *_data; // true for busy bits, false for available
} bit_array;

/*
 * Constructor
 *
 * 1 is returned if:
 * memory allocation fails
 * <size> is invalid (negative)
 * bb is a null pointer
 */
int init_bit_array(int size, bit_array *bb);

/*
 * Destructor
 * Does nothing if bb is a null pointer
 *
 * frees the memory allocated for bb->_data
 * sets bb->_data to NULL
 * sets bb->_size to 0
 */
void destroy_bit_array(bit_array *bb);

/*
 * setter methods
 *
 * for all of the following setter methods:
 *   1 is returned if: wrong indexes are given, or the bitset pointer is NULL
 *   0 if success
 *
 * Convention for intervals start, end is:
 * start and end inclusive
 * some_operation (start, end, bitarray) operates on start, start+1, ... end-1,
 * end
 *
 */
int set_range_busy(size_t start, size_t end,
                   bit_array *bb); // sets those bits to 1 (true)
int set_range_free(size_t start, size_t end,
                   bit_array *bb);           // sets those bits to 0 (false)
int set_bit_busy(size_t bit, bit_array *bb); // sets that bit to 1 (true)
int set_bit_free(size_t bit, bit_array *bb); // sets that bit to 0 (false)

/*
 * Availability queries
 * For all of the following querying methods
 *
 * -1 is returned if index is wrong or if the pointer is null
 *  0 is returned if bit or range is NOT available
 *  1 is returned if bit or range is available
 *
 *  convention for ranges start, end is:
 *  some_query(start, end, bitarray) queries on bits start, start+1, ... end-1,
 * end
 */

/*
 * _any_in_range
 * This is the primitive function for availability query
 * scans the range for any busy bits
 *
 * returns -1 if parameter [start, end] is invalid
 * OR if the pointer bb is NULL
 *
 * returns 1 if range [start, ... end-1, end] contains 1
 * returns 0 otherwise
 */
int _any_in_range(size_t start, size_t end, bit_array *bb);

/*
 * Checks validity of the parameters [0 <= start < end < bb->_sze, bb != NULL]
 * and returns !_any_in_range(start, end, bb)
 */
int is_range_available(size_t start, size_t end, bit_array *bb);

/*
 * Checks validity of the parameter i and returns
 * !_any_in_range(i, i, bb)
 */
int is_bit_available(size_t i, bit_array *bb);

/*
 * Finds the next bit with value 1 (true)
 */

/*
 * Operations between arrays
 *
 * Return values:
 * For all these operations
 * 1 is returned if one of the following errors occur:
 * - the first or the second or the out bit array pointer is NULL
 * - the sizes of the first, second and out don't match
 * - the data of any of the three bit arrays is NULL
 *
 * 0 is returned if the operation work successfully
 */
int logical_and(bit_array *first, bit_array *second, bit_array *out);

/*
 * Printing contents of a bit array
 *
 * print format:
 * for each busy range start, start+1, ... start+ N-1 = end:
 * start-end (N bits)
 *
 * does nothing if the pointer to a bit array is NULL
 *
 * does not print a newline at the end
 */
void print_bit_array(bit_array *bb);

#endif
