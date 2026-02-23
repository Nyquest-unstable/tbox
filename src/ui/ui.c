
#include "tbox/tbox.h"
#include "backlight.h"

#ifdef TB_CONFIG_PACKAGE_HAVE_LVGL
#   include "lvgl/lvgl.h"
#endif

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    tb_printf("TBOX UI应用启动...\n");
#ifndef TB_CONFIG_PACKAGE_HAVE_LVGL
    // 原有背光控制代码
    backlight_t bl;

    if (!tb_init(tb_null, tb_null)) {
        return -1;
    }

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
#else
    // LVGL演示代码
    if (!tb_init(tb_null, tb_null)) {
        tb_printf("初始化TBOX失败\n");
        return -1;
    }

    tb_printf("UI应用启动 (LVGL模式)...\n");

    // 初始化LVGL
    lv_init();

    tb_printf("LVGL初始化成功\n");
    
    // 这里可以添加具体的LVGL GUI代码
    // 创建一个简单的标签作为演示
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello LVGL with TBOX!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    tb_printf("已创建LVGL标签\n");
    tb_printf("LVGL演示运行成功!\n");

    // 清理资源
    lv_deinit();
#endif

    tb_exit();
    return 0;
}

