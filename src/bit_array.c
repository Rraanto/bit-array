/*
 * Implementation of library bit_array
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bit_array.h"

int init_bit_array(int size, bit_array *bb) {
	bb->_size = size; 
	bb->_data = (bool*) calloc(size, sizeof(bool));

	if (bb->_data == NULL) return 1;
	return 0; 
}

void destroy_bit_array(bit_array *bb) {
	free(bb->_data); 
}

/*
 * Setters 
 */
int set_range_busy(size_t start, size_t end, bit_array *bb) {
	/*
	 * Sets the range [start, end] (both included) to true (busy) 
	 * 1 is returned if and only if invalid interval 
	 * 0 is returned when success
	 */
	if (start < 0 || end >= bb->_size || end - start >= bb->_size) return 1; 
	for (int i = start; i <= end; i++) bb ->_data[i] = true; 
	return 0; 
}

int set_range_free(size_t start, size_t end, bit_array *bb) {
	/*
	 * Sets the range [start, end] (both included) to false (available)
	 * 1 is returned if and only if invalid interval 
	 * 0 is returned when success
	 */
	if (start < 0 || end >= bb ->_size || end - start >= bb ->_size) return 1; 
	for(int i = start; i <= end; i++) bb -> _data[i] = false; 
	return 0; 
}

// Wrappers for single bit queries
int set_bit_busy(size_t i, bit_array *bb) {return set_range_busy(i, i, bb); }
int set_bit_free(size_t i, bit_array *bb) {return set_range_free(i, i, bb); }

/*
 * Queries
 */
int is_range_available(size_t start, size_t end, bit_array *bb) {
	// returns -1 if i is invalid 
	// returns 1 if range is fully available 
	// returns 0 if range is not fully available 
	if (end <= start || start < 0 || end > bb->_size) return -1; 
	bool res = true; 
	size_t i = 0; 
	while (res && i < start - end) res = !bb->_data[i]; i++; 
	return res; 
}

int is_bit_available(size_t i, bit_array *bb) {
	return is_range_available(i, i, bb);
}

