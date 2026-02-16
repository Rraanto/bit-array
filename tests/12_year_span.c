#include <stdio.h>
/*
 * This tests an array of 10 * (366 * 24 * 60): a toy calendar
 * of 10 years with periodic busy groups (=1)
 */

#include "bit_array.h"

#define TABLE_SIZE 5270400

int main() {
  bit_array table;
  init_bit_array(TABLE_SIZE, &table);

  int success;
  int minutes_per_day = 24 * 60;
  for (int year = 0; year < 10; year++) {
    for (int day = 0; day < 366; day++) {
      int start = (year * 366 + day) * minutes_per_day;
      int start_minute = start + 60 * 9;
      int end_minute = start + 60 * 17;

      success = set_range_busy(start_minute, end_minute, &table) !=
                1; // 8(AM) - 17(PM)
      if (!success) {
        printf("Failure to set range %d-%d busy\n", start_minute, end_minute);
        break;
      }
    }
  }

  print_bit_array(&table);

  if (is_range_available(0, table._size - 1, &table) == 1) {
    printf("Table is empty\n");
  }

  destroy_bit_array(&table);
}
