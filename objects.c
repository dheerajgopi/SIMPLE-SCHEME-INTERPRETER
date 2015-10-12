#include "header.h"

/* prototypes -------------------------------------- */
object *create_object();
char is_empty_list(object *obj);
char is_bool(object *obj);
char is_false(object *obj);
char is_true(object *obj);
object *create_sym_obj(char *value);
char is_symbol(object *obj);
object *create_num_obj(long value);
char is_num(object *obj);
object *create_char_obj(char value);
char is_character(object *obj);
object *create_str_obj(char *value);
char is_string(object *obj);
object *cons(object *car, object *cdr);
object *car(object *pair);
void set_car(object *obj, object *value);
object *cdr(object *pair);
void set_cdr(object *obj, object *value);
char is_pair(object *obj);
/* ------------------------------------------------- */

/* creates an object using malloc
 * exits if malloc fails */
object *create_object()
{
    object *obj;

    obj = malloc(sizeof(object));
    if (obj == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    return obj;
}

object *empty_list;
object *false;  // boolean false
object *true;   // boolean true
object *symbol_table;

/* check if empty list */
char is_empty_list(object *obj)
{
    return obj->type == EMPTY_LIST;
}

/* check if boolean object */
char is_bool(object *obj)
{
    return obj->type == BOOL;
}

/* check if false */
char is_false(object *obj)
{
    return obj == false;
}

/* check if true */
char is_true(object *obj)
{
    return !is_false(obj);
}

/* returns a symbol object.
 * Also adds to symbol table if symbol is not in the table */
object *create_sym_obj(char *value)
{
    object *obj;
    object *element;

    /* search for symbol in symbol table */
    element = symbol_table;
    while (!is_empty_list(element)) {
        if (strcmp(element->data.symbol.value, value) == 0)
            return car(element);
        element = cdr(element);
    }

    /* else create symbol and add to symbol table */
    obj = create_object();
    obj->type = SYMBOL;
    obj->data.symbol.value = malloc(strlen(value) + 1);
    if (obj->data.symbol.value == NULL) {
        fprintf(stderr, "malloc failed! out of memory.\n");
        exit(1);
    }
    strcpy(obj->data.symbol.value, value);
    symbol_table = cons(obj, symbol_table);
    return obj;
}

/* check if object is a symbol */
char is_symbol(object *obj)
{
    return obj->type == SYMBOL;
}

/* creates a number object
 * type field is set as NUM
 * data field is set as "value" */
object *create_num_obj(long value)
{
    object *obj;

    obj = create_object();
    obj->type = NUM;
    obj->data.int_num.value = value;

    return obj;
}

/* returns object_type enum value
 * to check if its a NUM data type */
char is_num(object *obj)
{
    return obj->type == NUM;
}

/* make character object */
object *create_char_obj(char value)
{
    object *obj;

    obj = create_object();
    obj->type = CHARACTER;
    obj->data.character.value = value;

    return obj;
}

/* check if type is CHARACTER */
char is_character(object *obj)
{
    return obj->type == CHARACTER;
}

/* create a string object */
object *create_str_obj(char *value)
{
    object *obj;

    obj = create_object();
    obj->type = STRING;
    obj->data.string.value = malloc(strlen(value) + 1); // for '\0'
    if (obj->data.string.value == NULL) {
        fprintf(stderr, "malloc failed! out of memory.\n");
        exit(1);
    }
    strcpy(obj->data.string.value, value);
    return obj;
}

/* check if the object is of string datatype */
char is_string(object *obj)
{
    return obj->type == STRING;
}

/* constructor function for creating a pair */
object *cons(object *car, object *cdr)
{
    object *obj;

    obj = create_object();
    obj->type = PAIR;
    obj->data.pair.car = car;
    obj->data.pair.cdr = cdr;
    return obj;
}

/* check if the object is of pair datatype */
char is_pair(object *obj)
{
    return obj->type == PAIR;
}

/* returns car object from the pair */
object *car(object *pair)
{
    return pair->data.pair.car;
}

/* sets car value of the given pair */
void set_car(object *obj, object *value)
{
    obj->data.pair.car = value;
}

/* returns cdr object from the pair */
object *cdr(object *pair)
{
    return pair->data.pair.cdr;
}

/* sets cdr value of the given pair */
void set_cdr(object *obj, object *value)
{
    obj->data.pair.cdr = value;
}
