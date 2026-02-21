
#include "tbox/tbox.h"

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    if (!tb_init(tb_null, tb_null))
        return -1;

    tb_printf("Hello World!\n");

    tb_exit();
    return 0;
}

