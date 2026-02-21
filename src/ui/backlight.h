
#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include "tbox/tbox.h"

#define MAX_BACKLIGHT_PATH 256

typedef struct {
    char backlight_path[MAX_BACKLIGHT_PATH];
    char max_brightness_path[MAX_BACKLIGHT_PATH];
    tb_int_t max_brightness;
} backlight_t;

tb_bool_t backlight_init(backlight_t* bl);
tb_void_t backlight_exit(backlight_t* bl);
tb_bool_t backlight_set_brightness(backlight_t* bl, tb_int_t brightness);
tb_int_t backlight_get_max_brightness(backlight_t* bl);
tb_bool_t backlight_turn_off(backlight_t* bl);
tb_bool_t backlight_turn_on(backlight_t* bl);

#endif

