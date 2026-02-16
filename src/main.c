#include <stdio.h> 
#include <stdbool.h>
#include <stdlib.h>

#include "bit_array.h"

int main() {
	bit_array bb;
	int init_success = init_bit_array(5, &bb) == 0;
	if (!init_success) printf("Bit array initialisation failed\n");

	set_bit_busy(2, &bb);
	set_range_busy(1, 3, &bb); 

	// setting everything back to free in two different ways 
	int success; 
	for (int i = -1; i < bb._size+1; i++) {
		success = !set_bit_free(i, &bb);
		if (!success) printf("Setting fail for minute %d\n", i);
	}

	printf("The size of the bit_array is: %d \n", bb._size);
	for (int i = 0; i < bb._size; i++) {
		printf("Minute %d: %s\n", i, is_bit_available(i, &bb)?"yes":"no");
	}

	destroy_bit_array(&bb);
	return 0;
}
