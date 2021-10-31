#include <stdio.h>
#include <stdlib.h>
#include "char_buff.h"

int main() {
	for (;;) {
		char_buff_t *test = cb_create();

		cb_append(test, "%i, %s");
		cb_appendf(test, test->data, 5, "mad capn toooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooom");
		char_buff_t *clone = cb_create();
		cb_append(clone, cb_export(test));

		cb_println(test);
		cb_println(clone);
		
		
		
		cb_free(clone);
		cb_free(test);
		break;
	}


	

	return 0;
}