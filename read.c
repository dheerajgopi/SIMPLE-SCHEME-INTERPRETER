#include "header.h"
#include "objects.c"

/* prototypes -------------------------------------- */
object *read(FILE *in);
object *read_character(FILE *in);
object *read_pair(FILE *in);
char is_initial(int c);
void special_character(FILE *in, char *str);
void peek_expected_delimiter(FILE *in);
char is_delimiter(int c);
int peek(FILE *in);
void remove_whitespace(FILE *in);
/* ------------------------------------------------- */

/* reads from stdin and returns the objects */
object *read(FILE *in)
{
    int c;
    int sign = 1;
    int i;
    long num = 0;
    char buf[BUFFER_MAX];

    remove_whitespace(in);

    c = getc(in);

    if (c == '#') {  // boolean or character
        c = getc(in);
        switch (c) {
        case 't':
            return true;
        case 'f':
            return false;
        case '\\':
            return read_character(in);
        default:
            fprintf(stderr, "unknown boolean or character\n");
            exit(1);
        }
    }
    else if (isdigit(c) || (c == '-' && (isdigit(peek(in))))) { // num
        if (c == '-')
            sign = -1;
        else
            ungetc(c, in);

        while (isdigit(c = getc(in)))   // form the number
            num = (num * 10) + (c - '0');

        num *= sign;

        if (is_delimiter(c)) {
            ungetc(c, in);
            return create_num_obj(num);
        }
        else {
            fprintf(stderr, "num not followed by delimiter\n");
            exit(1);
        }
    }
    else if (c == '"'){
        i = 0;

        while((c = getc(in)) != '"') {
            if (c == '\\') {
                if ((c = getc(in)) == 'n')
                    c = '\n';
            }
            if (c == EOF) {
                fprintf(stderr, "string literal not terminated!\n");
                exit(1);
            }
            if (i < BUFFER_MAX - 1) // for '\0'
                buf[i++] = c;
            else {
                fprintf(stderr, "string too long. Max length is %d\n",
                                BUFFER_MAX);
                exit(1);
            }
        }
        buf[i] = '\0';
        return create_str_obj(buf);
    }
    else if (c == '(') {    // empty list or pair
        return read_pair(in);
    }
    else {
        fprintf(stderr, "bad input, number expected\n");
        exit(1);
    }

    fprintf(stderr, "read operation failed\n");
    exit(1);
}

/* reads character, also handles special characters */
object *read_character(FILE *in)
{
    int c;

    c = getc(in);

    switch(c) {
    case EOF:
        fprintf(stderr, "incomplete character literal.\n");
        exit(1);
    case 's':
        if (peek(in) == 'p') {
            special_character(in, "pace");
            peek_expected_delimiter(in);
            return create_char_obj(' ');
        }
        break;
    case 'n':
        if (peek(in) == 'e') {
            special_character(in, "ewline");
            peek_expected_delimiter(in);
            return create_char_obj('\n');
        }
        break;
    }
    peek_expected_delimiter(in);
    return create_char_obj(c);
}

/* read a pair datatype */
object *read_pair(FILE *in)
{
    int c;
    object *car_obj;
    object *cdr_obj;

    remove_whitespace(in);

    c = getc(in);
    if (c == ')')
        return empty_list;
    ungetc(c, in);

    car_obj = read(in);
    remove_whitespace(in);

    c = getc(in);
    if (c == '.') { // improper list
        c = getc(in);
        if (!is_delimiter(c)) {
            fprintf(stderr, "'.' not followed by delimiter.\n");
            exit(1);
        }

        cdr_obj = read(in);
        remove_whitespace(in);
        c = getc(in);
        if (c != ')') {
            fprintf(stderr, "right parantheses missing!\n");
            exit(1);
        }
        return cons(car_obj, cdr_obj);
    }
    else {  // list
        ungetc(c, in);
        cdr_obj = read_pair(in);
        return cons(car_obj, cdr_obj);
    }
}

/* checking special characters */
void special_character(FILE *in, char *str)
{
    int c;

    while (*str != '\0') {
        c = getc(in);
        if (c != *str) {
            fprintf(stderr, "unknown character '%c'\n", c);
            exit(1);
        }
        str++;
    }
}

void peek_expected_delimiter(FILE *in)
{
    if (!is_delimiter(peek(in))) {
        fprintf(stderr, "character not followed by delimiter.\n");
        exit(1);
    }
}

/* returns true if the char argument is
 * one of the delimiters */
char is_delimiter(int c)
{
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
           c == '"'   || c == ';';
}

/* to check if symbol has valid first character */
char is_initial(int c) {
        return isalpha(c) || c == '*' || c == '/' || c == '>' ||
               c == '<'   || c == '=';
}

/* get next char in the stream */
int peek(FILE *in)
{
    int c;

    c = getc(in);
    ungetc(c, in);

    return c;
}

/* eats whitespace at the beginning, also discards comments after ';' */
void remove_whitespace(FILE *in)
{
    int c;

    while ((c = getc(in)) != EOF) {
        if (isspace(c))
            continue;
        else if (c == ';') {    // comments
            while (((c = getc(in))) != EOF && c != '\n');
            continue;
        }

        ungetc(c, in);
        break;
    }
}
