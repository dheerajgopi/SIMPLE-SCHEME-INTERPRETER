#ifndef HDR
#define HDR

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_MAX 1000

typedef enum {EMPTY_LIST, BOOL, SYMBOL, NUM, CHARACTER,
              STRING, PAIR} object_type;

typedef struct object {
    object_type type;
    union {
        struct {
            char value;
        } boolean;
        struct {
            char *value;
        } symbol;
        struct {
            long value;
        } int_num;
        struct {
            char value;
        } character;
        struct {
            char *value;
        } string;
        struct {
            struct object *car;
            struct object *cdr;
        } pair;
    } data;
} object;

#endif
