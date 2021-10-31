#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "char_buff.h"


static cb_status_t resize_extend_only(char_buff_t *cb, size_t new_capacity) {
    char *newData = realloc(cb->data, new_capacity + 1);
    if (NULL == newData) return CB_ERROR_REALLOC_FAILURE;

    cb->data = newData;
    cb->capacity = new_capacity;
    return CB_SUCCESS;
}

static cb_status_t expand(char_buff_t *cb) {
    size_t newCapacity;
    if (cb->capacity <= 0)
        newCapacity = 1;
    else
        newCapacity = cb->capacity * 2;
    
    return resize_extend_only(cb, newCapacity);
}

static cb_status_t append_char_no_terminator(char_buff_t *cb, char c) {
    while (cb->length >= cb->capacity) {
        int expand_result = expand(cb);
        if (expand_result) return expand_result;
    }
    
    cb->data[cb->length++] = c;
    return CB_SUCCESS;
}

static void add_terminator(char_buff_t *cb) {
    cb->data[cb->length] = '\0';
}

static size_t get_initial_capacity() {
    return 2000u;
}

static bool is_string_from_self(char_buff_t *cb, char *str) {
    return cb->data <= str && str <= cb->data + cb->length;
}



char_buff_t *cb_create_with_capacity(size_t capacity) {
    char_buff_t *result = (char_buff_t *)malloc(sizeof(char_buff_t));

    if (NULL == result)
        return NULL;

    result->data = (char *)malloc(sizeof(char) * (capacity + 1));

    if (NULL == result->data) {
        free(result);
        return NULL;
    }

    result->data[0] = '\0';
    result->capacity = capacity;
    result->length = 0;
    return result;
}

char_buff_t *cb_create() {
    return cb_create_with_capacity(get_initial_capacity());
}

void cb_free(char_buff_t *cb) {
    if (NULL != cb) {
        free(cb->data);
        free(cb);
    }
}


cb_status_t cb_append_char(char_buff_t *cb, char c) {
    if (NULL == cb) return false;
    cb_status_t status = append_char_no_terminator(cb, c);

    add_terminator(cb);
    return status;
}

cb_status_t cb_append(char_buff_t *cb, char *str) {
    if (NULL == cb || NULL == str) return CB_ERROR_NULL_ARGUMENT;
    if ('\0' == str[0]) return CB_SUCCESS;

    // check for appending self
    if (is_string_from_self(cb, str)) {
        char_buff_t *cpy = cb_initialize(str);
        cb_status_t status = cb_append(cb, cpy->data);

        cb_free(cpy);
        return status;
    }

    if (is_string_from_self(cb, str)) {
        char_buff_t *cpy = cb_initialize(str);
        cb_status_t status = cb_append(cb, cpy->data);

        cb_free(cpy);
        return status;
    }

    cb_status_t status = CB_SUCCESS;
    char c;
    size_t i = 0;
    while ((c = str[i++]) != '\0') {
        status = append_char_no_terminator(cb, c);
        if (status) break;
    }

    add_terminator(cb);
    return status;
}

cb_status_t cb_append_substr(char_buff_t *cb, char *str, size_t start, size_t length) {
    if (NULL == cb || NULL == str) return CB_ERROR_NULL_ARGUMENT;
    if ('\0' == str[0]) return CB_SUCCESS;

    // check for appending self
    if (is_string_from_self(cb, str)) {
        char_buff_t *cpy = cb_initialize(str);
        cb_status_t status = cb_append_substr(cb, cpy->data, start, length);

        cb_free(cpy);
        return status;
    }

    int status = 0;
    char c;
    for (size_t i = start; i - start < length; ++i) {
        c = str[i];
        if (c == '\0') break;
        status = append_char_no_terminator(cb, c);
        if (status) break;
    }

    add_terminator(cb);
    return status;
}

cb_vappendf(char_buff_t *cb, char *format, va_list args) {
    //check for null arguments
    if (NULL == cb || NULL == format || NULL == args)
        return CB_ERROR_NULL_ARGUMENT;

    // check for appending self
    if (is_string_from_self(cb, format)) {
        char_buff_t *cpy = cb_initialize(format);
        cb_status_t status = cb_vappendf(cb, cpy->data, args);

        cb_free(cpy);
        return status;
    }

    cb_status_t status = CB_SUCCESS;
    char *end;
    size_t char_limit;

retry:
    end = cb->data + cb->length;
    char_limit = cb->capacity - cb->length;

    int char_count = vsnprintf(end, char_limit + 1, format, args);

    // check the result to see if the append was successful.
    // If not: was it an encoding error?
    // if not: was the buffer not big enouph?
    // if so: expand the buffer and try again.
    if (char_count < 0) {
        status = CB_ERROR_ENCODING_ERROR;
    }
    else if (char_count > char_limit) {
        // buffer not big enouph so make it big enouph and try again

        // making the buffer twice as big plus digitcount may be a bit much
        // but it ensures there will be leading space for future appending
        // and that there will be enouph space for the retry.
        status = resize_extend_only(cb, cb->capacity * 2 + char_count);
        if (!status)
            goto retry;
    }
    else {
        cb->length += char_count;
    }

    add_terminator(cb);
    return status;
}

cb_status_t cb_appendf(char_buff_t *cb, char *format, ...) {
    // initialize variable args list
    va_list args;
    va_start(args, format);

    cb_status_t status = cb_vappendf(cb, format, args);
    
    va_end(args);
    add_terminator(cb);
    return status;
}

cb_status_t cb_append_int(char_buff_t *cb, int i) {
    return cb_appendf(cb, "%i", i);
}

cb_status_t cb_appendln(char_buff_t *cb, char *str) {
    cb_status_t status = CB_SUCCESS;
    status = cb_append(cb, str);
    if (status) return status;
    status = cb_append_char(cb, '\n');
    return status;
}

cb_status_t cb_clear(char_buff_t *cb) {
    if (NULL == cb) return CB_ERROR_NULL_ARGUMENT;
    cb->data[0] = '\0';
    cb->length = 0;
    return CB_SUCCESS;
}

cb_status_t cb_print(char_buff_t *cb) {
    if (NULL == cb) return CB_ERROR_NULL_ARGUMENT;
    printf("%s", cb->data);
    return CB_SUCCESS;
}

cb_status_t cb_println(char_buff_t *cb) {
    if (NULL == cb) return CB_ERROR_NULL_ARGUMENT;
    printf("%s\n", cb->data);
    return CB_SUCCESS;
}

char_buff_t *cb_initialize(char *str) {
    char_buff_t *result = cb_create();
    if (NULL == result) return NULL;

    if (!cb_append(result, str)){
        return result;
    }
    else {
        cb_free(result);
        return NULL;
    }
}

char_buff_t *cb_clone(char_buff_t *cb) {
    if (NULL == cb) return NULL;

    char_buff_t *result = cb_create();
    if (NULL == result) return NULL;

    if (cb_append(result, cb->data)) {
        cb_free(result);
        return NULL;
    }
    else return result;
}

char *cb_export(char_buff_t *cb) {
    if (NULL == cb) return NULL;

    char *result = (char *)malloc(sizeof(char) * (cb->length + 1));
    if (NULL == result)
        return CB_ERROR_MALLOC_FAILURE;

    strncpy_s(result, cb->length + 1, cb->data, cb->length + 1);

    return result;
}


//
//cb_tokenizer_t cb_tokenize(char_buff_t *cb, const char *delim) {
//    return (cb_tokenizer_t) { cb, delim, NULL };
//}
//
//char *cb_next_token(cb_tokenizer_t *tokenizer) {
//    if (NULL == tokenizer->checkpoint)
//        return strtok_s()
//        return strtok_r(tokenizer->cb->data, tokenizer->delim, NULL);
//    else
//        return strtok_r(NULL, tokenizer->delim, tokenizer->checkpoint);
//}