#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum cb_status {
    CB_SUCCESS = 0,
    CB_ERROR,
    CB_ERROR_MALLOC_FAILURE,
    CB_ERROR_REALLOC_FAILURE,
    CB_ERROR_NULL_ARGUMENT,
    CB_ERROR_CREATION_FAILURE,
    CB_ERROR_INDEX_OUT_OF_BOUNDS,
    CB_ERROR_ENCODING_ERROR,
} cb_status_t;

typedef struct char_buff {
    // the string stored in the buffer. Is null terminated and 1 byte longer than capacity (for the null terminator)
    char *data;
    // maximum number of characters. Gets extended as needed.
    size_t capacity;
    // character count
    size_t length;
} char_buff_t;



char_buff_t *cb_create_with_capacity(size_t capacity);

char_buff_t *cb_create();

void cb_free(char_buff_t *cb);


cb_status_t cb_append_char(char_buff_t *cb, char c);

cb_status_t cb_append(char_buff_t *cb, char *str);

cb_status_t cb_append_substr(char_buff_t *cb, char *str, size_t start, size_t length);

cb_status_t cb_appendln(char_buff_t *cb, char *str);

cb_status_t cb_appendf(char_buff_t *cb, char *format, ...);

cb_status_t cb_append_int(char_buff_t *cb, int i);

cb_status_t cb_clear(char_buff_t *cb);

cb_status_t cb_print(char_buff_t *cb);

cb_status_t cb_println(char_buff_t *cb);

char_buff_t *cb_initialize(char *str);

char_buff_t *cb_clone(char_buff_t *cb);

char *cb_export(char_buff_t *cb);