#include "header.h"
#include "read.c"
#include "print.c"

/* initialise */
void init()
{
    empty_list = create_object();
    empty_list->type = EMPTY_LIST;

    false = create_object();
    false->type = BOOL;
    false->data.boolean.value = 0;

    true = create_object();
    true->type = BOOL;
    true->data.boolean.value = 1;
}

/* --------------------------------------------------------- */

/* evaluates the objects read */
object *eval(object *expr)
{
    return expr;
}

/* REPL */
int main()
{
    printf("Hello! Press CTRL+C to exit.\n");

    init();

    while (1) {
        printf(">");
        write(eval(read(stdin)));
        printf("\n");
    }

    return 0;
}
