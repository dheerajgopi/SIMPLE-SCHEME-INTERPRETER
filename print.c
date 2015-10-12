#include "header.h"

/* prototypes -------------------------------------- */
void write(object *obj);
void write_pair(object *pair);
/* ------------------------------------------------- */

/* print */
void write(object *obj)
{
    char c;
    char *str;

    switch (obj->type) {
    case EMPTY_LIST:
        printf("()");
        break;
    case BOOL:
        printf("#%c", is_false(obj) ? 'f' : 't');
        break;
    case NUM:
        printf("%ld", obj->data.int_num.value);
        break;
    case CHARACTER:
        c = obj->data.character.value;
        printf("#\\");
        switch(c) {
        case '\n':
            printf("newline");
            break;
        case ' ':
            printf("space");
            break;
        default:
            putchar(c);
        }
        break;
    case STRING:
        str = obj->data.string.value;
        putchar('"');
        while (*str != '\0') {
            switch (*str) {
            case '\n':
                printf("\\n");
                break;
            case '\\':
                printf("\\\\");
                break;
            case '"':
                printf("\\\"");
                break;
            default:
                putchar(*str);
            }
            str++;
        }
        putchar('"');
        break;
    case PAIR:
        printf("(");
        write_pair(obj);
        printf(")");
        break;
    default:
        fprintf(stderr, "unknown type\n");
        exit(1);
    }
}

/* for printing a pair object */
void write_pair(object *obj)
{
    object *car_obj;
    object *cdr_obj;

    car_obj = car(obj);
    cdr_obj = cdr(obj);

    write(car_obj);
    if (cdr_obj->type == PAIR) {
        printf(" ");
        write_pair(cdr_obj);
    }
    else if (cdr_obj->type == EMPTY_LIST)
        return;
    else {
        printf(" . ");
        write(cdr_obj);
    }
}
