#include <stdio.h>
#include <stdlib.h>
#include "char_buff.h"

int main() {
	for (;;) {
		char_buff_t *test = cb_create();

		cb_append(test, "0123456789");
		cb_append(test, test->data + 4);

		

		
		printf("%s", test->data);

		
		
		
		
		cb_free(test);
		break;
	}


	

	return 0;
}