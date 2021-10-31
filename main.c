#include <stdio.h>
#include <stdlib.h>
#include "char_buff.h"

int main() {
	/*char_buff_t *test = cb_create();
	printf("%i", test->capacity);

	cb_append(test, "abcd");
	cb_append(test, "\nHello World!");
	cb_append(test, "\nAll work and no play makes jack a dull boy.");
	cb_append(test, "\n");

	for (int i = 0; i < 100; ++i)
		cb_append(test, "All work and no play makes jack a dull boy.\n");

	cb_println(test);

	printf("%i", test->capacity);
	cb_free(test);*/
	char_buff_t *test = cb_create();
	for (int i = 0; i < 1996; ++i)
		cb_append_char(test,'f');
	cb_append_char(test, ':');

	printf("%i\n", test->capacity);

	cb_append_int(test, 2000);


	printf("%i\n", test->capacity);
	cb_println(test);
	cb_free(test);


	

	return 0;
}