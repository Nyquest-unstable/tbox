
#include "tbox/tbox.h"
#include "backlight.h"

#ifdef TB_CONFIG_PACKAGE_HAVE_LVGL
#   include "lvgl/lvgl.h"
#endif

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    tb_printf("TBOX UI应用启动...\n");

    // 初始化TBOX
    if (!tb_init(tb_null, tb_null)) {
        return -1;
    }

#ifdef TB_CONFIG_PACKAGE_HAVE_LVGL
    tb_printf("LVGL功能已启用，需要配置显示驱动\n");
    tb_printf("提示：目前项目尚未实现LVGL驱动移植代码\n");
#endif

    // 背光控制代码（无论是否启用LVGL都保留）
    backlight_t bl;

    tb_printf("UI应用启动...\n");

    if (!backlight_init(&bl)) {
        tb_printf("初始化背光控制失败\n");
        tb_exit();
        return -1;
    }

    tb_printf("背光控制初始化成功\n");
    tb_printf("最大亮度值: %d\n", backlight_get_max_brightness(&bl));

    if (argc > 1) {
        if (tb_strcmp(argv[1], "on") == 0) {
            tb_printf("正在开启LCD背光...\n");
            if (backlight_turn_on(&bl)) {
                tb_printf("LCD背光已开启\n");
            }
        } else if (tb_strcmp(argv[1], "off") == 0) {
            tb_printf("正在关闭LCD背光...\n");
            if (backlight_turn_off(&bl)) {
                tb_printf("LCD背光已关闭\n");
            }
        } else {
            tb_int_t brightness = tb_atoi(argv[1]);
            tb_printf("正在设置LCD亮度为: %d\n", brightness);
            if (backlight_set_brightness(&bl, brightness)) {
                tb_printf("LCD亮度已设置\n");
            }
        }
    } else {
        tb_printf("使用方法:\n");
        tb_printf("  ui on    - 开启背光\n");
        tb_printf("  ui off   - 关闭背光\n");
        tb_printf("  ui <num> - 设置亮度值\n");
    }

    backlight_exit(&bl);

    tb_exit();
    return 0;
}

