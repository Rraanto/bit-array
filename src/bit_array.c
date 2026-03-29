/*
 * Implementation of library bit_array
 * Uses dense 64-bit word storage for efficiency
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_array.h"

int init_bit_array(int size, bit_array *bb) {
  if (bb == NULL)
    return 1;

  // initialises the size to 0 (without data)
  if (size <= 0) {
    bb->_size = (size_t)0;
    bb->_num_words = (size_t)0;
    bb->_data = NULL;
    return 1; // empty
  }

  // Calculate number of 64-bit words needed
  bb->_size = (size_t)size;
  bb->_num_words = ((size_t)size + 63) / 64;

  // allocates memory for the words (initialized to 0 = all bits free)
  bb->_data = (uint64_t *)calloc(bb->_num_words, sizeof(uint64_t));

  if (bb->_data == NULL) {
    bb->_num_words = 0;
    return 1;
  }

  return 0;
}

void destroy_bit_array(bit_array *bb) {
  if (bb == NULL)
    return;

  // frees the memory allocated for the bit_array data
  free(bb->_data);
  bb->_data = NULL;
  bb->_size = 0;
  bb->_num_words = 0;
}

size_t bit_array_size(const bit_array *bb) { return bb->_size; }

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

  size_t start_word = WORD_INDEX(start);
  size_t end_word = WORD_INDEX(end);
  size_t start_bit = BIT_INDEX(start);
  size_t end_bit = BIT_INDEX(end);

  if (start_word == end_word) {
    // Range is within a single word
    uint64_t mask = ((1ULL << (end - start + 1)) - 1) << start_bit;
    bb->_data[start_word] |= mask;
  } else {
    // Range spans multiple words
    // First partial word (from start_bit to end of word)
    uint64_t first_mask = (~0ULL) << start_bit;
    bb->_data[start_word] |= first_mask;

    // Middle full words (all bits set)
    for (size_t w = start_word + 1; w < end_word; w++) {
      bb->_data[w] = ~0ULL;
    }

    // Last partial word (from start of word to end_bit)
    uint64_t last_mask = (1ULL << (end_bit + 1)) - 1;
    bb->_data[end_word] |= last_mask;
  }

  return 0;
}

int set_range_free(size_t start, size_t end, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return 1;
  if (start > end)
    return 1;
  if (end >= bb->_size)
    return 1;

  size_t start_word = WORD_INDEX(start);
  size_t end_word = WORD_INDEX(end);
  size_t start_bit = BIT_INDEX(start);
  size_t end_bit = BIT_INDEX(end);

  if (start_word == end_word) {
    // Range is within a single word
    uint64_t mask = ((1ULL << (end - start + 1)) - 1) << start_bit;
    bb->_data[start_word] &= ~mask;
  } else {
    // Range spans multiple words
    // First partial word
    uint64_t first_mask = (~0ULL) << start_bit;
    bb->_data[start_word] &= ~first_mask;

    // Middle full words (all bits cleared)
    for (size_t w = start_word + 1; w < end_word; w++) {
      bb->_data[w] = 0;
    }

    // Last partial word
    uint64_t last_mask = (1ULL << (end_bit + 1)) - 1;
    bb->_data[end_word] &= ~last_mask;
  }

  return 0;
}

// Wrappers for single bit queries
int set_bit_busy(size_t i, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return 1;
  if (i >= bb->_size)
    return 1;

  bb->_data[WORD_INDEX(i)] |= BIT_MASK(i);
  return 0;
}

int set_bit_free(size_t i, bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return 1;
  if (i >= bb->_size)
    return 1;

  bb->_data[WORD_INDEX(i)] &= ~BIT_MASK(i);
  return 0;
}

/*
 * Queries
 */
int _any_in_range(size_t start, size_t end, const bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (start > end)
    return -1;
  if (end >= bb->_size)
    return -1;

  size_t start_word = WORD_INDEX(start);
  size_t end_word = WORD_INDEX(end);
  size_t start_bit = BIT_INDEX(start);
  size_t end_bit = BIT_INDEX(end);

  if (start_word == end_word) {
    // Range is within a single word
    uint64_t mask = ((1ULL << (end - start + 1)) - 1) << start_bit;
    return (bb->_data[start_word] & mask) != 0 ? 1 : 0;
  } else {
    // First partial word
    uint64_t first_mask = (~0ULL) << start_bit;
    if ((bb->_data[start_word] & first_mask) != 0)
      return 1;

    // Middle full words
    for (size_t w = start_word + 1; w < end_word; w++) {
      if (bb->_data[w] != 0)
        return 1;
    }

    // Last partial word
    uint64_t last_mask = (1ULL << (end_bit + 1)) - 1;
    if ((bb->_data[end_word] & last_mask) != 0)
      return 1;

    return 0;
  }
}

int is_range_available(size_t start, size_t end, const bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (start > end)
    return -1;
  if (end >= bb->_size)
    return -1;

  // use _any_in_range to query for freeness
  int any = _any_in_range(start, end, bb);
  if (any < 0)
    return -1;
  return (any == 0);
}

int is_bit_available(size_t i, const bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (i >= bb->_size)
    return -1;

  return (bb->_data[WORD_INDEX(i)] & BIT_MASK(i)) == 0 ? 1 : 0;
}

int find_next_busy_bit(size_t start, const bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return -1;
  if (start >= bb->_size)
    return -1;

  size_t start_word = WORD_INDEX(start);
  size_t start_bit = BIT_INDEX(start);

  // Check the first word (starting from start_bit)
  uint64_t first_word = bb->_data[start_word];
  if (first_word != 0) {
    // Mask out bits before start
    uint64_t mask = (~0ULL) << start_bit;
    uint64_t masked_word = first_word & mask;
    if (masked_word != 0) {
      // Find first set bit using __builtin_ctzll if available,
      // otherwise manual search
      int bit_pos;

      // use faster builtins when available
#ifdef __GNUC__
      bit_pos = __builtin_ctzll(masked_word);
#else
      bit_pos = 0;
      while ((masked_word & (1ULL << bit_pos)) == 0)
        bit_pos++;
#endif
      return (int)(start_word * 64 + bit_pos);
    }
  }

  // Check remaining full words
  for (size_t w = start_word + 1; w < bb->_num_words; w++) {
    if (bb->_data[w] != 0) {
      int bit_pos;
#ifdef __GNUC__
      bit_pos = __builtin_ctzll(bb->_data[w]);
#else
      bit_pos = 0;
      while ((bb->_data[w] & (1ULL << bit_pos)) == 0)
        bit_pos++;
#endif
      return (int)(w * 64 + bit_pos);
    }
  }

  // none found
  return (int)bb->_size;
}

/*
 * Operations between arrays
 */
int logical_and(const bit_array *first, const bit_array *second,
                bit_array *out) {
  if (first == NULL || second == NULL || out == NULL)
    return 1;
  if (first->_data == NULL || second->_data == NULL || out->_data == NULL)
    return 1;
  if (first->_size != second->_size || out->_size != first->_size)
    return 1;

  // per word logical and
  for (size_t i = 0; i < first->_num_words; i++) {
    out->_data[i] = first->_data[i] & second->_data[i];
  }
  return 0;
}

int logical_or(const bit_array *first, const bit_array *second,
               bit_array *out) {
  if (first == NULL || second == NULL || out == NULL)
    return 1;
  if (first->_data == NULL || second->_data == NULL || out->_data == NULL)
    return 1;
  if (first->_size != second->_size || out->_size != first->_size)
    return 1;

  // per-word logical or
  for (size_t i = 0; i < first->_num_words; i++) {
    out->_data[i] = first->_data[i] | second->_data[i];
  }
  return 0;
}

/*
 * Utility functions
 *
 * Does nothing if bit array pointer is NULL
 */
void print_bit_array(const bit_array *bb) {
  if (bb == NULL || bb->_data == NULL)
    return;

  size_t start = 0;
  int currently_busy = 0;
  for (size_t i = 0; i < bb->_size; i++) {
    bool busy = (bb->_data[WORD_INDEX(i)] & BIT_MASK(i)) != 0;

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
