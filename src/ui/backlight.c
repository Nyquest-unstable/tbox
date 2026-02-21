#include "backlight.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

static tb_bool_t backlight_find_device(backlight_t* bl)
{
    const tb_char_t* backlight_dir = "/sys/class/backlight/";
    const tb_char_t* brightness_file = "/brightness";
    const tb_char_t* max_brightness_file = "/max_brightness";
    DIR* dir;
    struct dirent* entry;

    dir = opendir(backlight_dir);
    if (dir == tb_null) {
        tb_printf("无法打开目录: %s, 错误: %s\n", backlight_dir, strerror(errno));
        return tb_false;
    }

    while ((entry = readdir(dir)) != tb_null) {
        if (tb_strcmp(entry->d_name, ".") != 0 && tb_strcmp(entry->d_name, "..") != 0) {
            tb_snprintf(bl->backlight_path, sizeof(bl->backlight_path), "%s%s%s", backlight_dir, entry->d_name, brightness_file);
            
            if (access(bl->backlight_path, F_OK) == 0) {
                tb_snprintf(bl->max_brightness_path, sizeof(bl->max_brightness_path), "%s%s%s", backlight_dir, entry->d_name, max_brightness_file);
                closedir(dir);
                return tb_true;
            }
        }
    }

    closedir(dir);
    return tb_false;
}

static tb_bool_t backlight_read_max_brightness(backlight_t* bl)
{
    tb_int_t fd;
    tb_char_t buffer[32];

    fd = open(bl->max_brightness_path, O_RDONLY);
    if (fd < 0) {
        tb_printf("无法读取最大亮度值，默认为255\n");
        bl->max_brightness = 255;
        return tb_false;
    }

    tb_memset(buffer, 0, sizeof(buffer));
    if (read(fd, buffer, sizeof(buffer) - 1) > 0) {
        bl->max_brightness = tb_atoi(buffer);
    } else {
        bl->max_brightness = 255;
    }

    close(fd);
    return tb_true;
}

tb_bool_t backlight_init(backlight_t* bl)
{
    if (!bl) return tb_false;

    tb_memset(bl, 0, sizeof(backlight_t));

    if (!backlight_find_device(bl)) {
        tb_strlcpy(bl->backlight_path, "/sys/class/graphics/fb0/blank", sizeof(bl->backlight_path));
        if (access(bl->backlight_path, F_OK) != 0) {
            tb_printf("未找到背光设备\n");
            return tb_false;
        }
        bl->max_brightness = 1;
    } else {
        backlight_read_max_brightness(bl);
    }

    return tb_true;
}

tb_void_t backlight_exit(backlight_t* bl)
{
    if (!bl) return;
    tb_memset(bl, 0, sizeof(backlight_t));
}

tb_bool_t backlight_set_brightness(backlight_t* bl, tb_int_t brightness)
{
    if (!bl) return tb_false;

    tb_int_t fd;
    tb_char_t buffer[32];
    tb_int_t len;

    if (tb_strstr(bl->backlight_path, "/blank") != tb_null) {
        fd = open(bl->backlight_path, O_WRONLY);
        if (fd < 0) {
            tb_printf("无法打开背光文件: %s, 错误: %s\n", bl->backlight_path, strerror(errno));
            return tb_false;
        }
        tb_snprintf(buffer, sizeof(buffer), "%d", brightness > 0 ? 0 : 1);
        len = tb_strlen(buffer);
        if (write(fd, buffer, len) < 0) {
            tb_printf("无法写入背光值: %s, 错误: %s\n", bl->backlight_path, strerror(errno));
            close(fd);
            return tb_false;
        }
        close(fd);
        return tb_true;
    }

    if (brightness < 0) brightness = 0;
    if (brightness > bl->max_brightness) brightness = bl->max_brightness;

    fd = open(bl->backlight_path, O_WRONLY);
    if (fd < 0) {
        tb_printf("无法打开背光文件: %s, 错误: %s\n", bl->backlight_path, strerror(errno));
        return tb_false;
    }

    len = tb_snprintf(buffer, sizeof(buffer), "%d", brightness);
    if (write(fd, buffer, len) < 0) {
        tb_printf("无法写入背光值: %s, 错误: %s\n", bl->backlight_path, strerror(errno));
        close(fd);
        return tb_false;
    }

    close(fd);
    return tb_true;
}

tb_int_t backlight_get_max_brightness(backlight_t* bl)
{
    if (!bl) return 255;
    return bl->max_brightness;
}

tb_bool_t backlight_turn_off(backlight_t* bl)
{
    return backlight_set_brightness(bl, 0);
}

tb_bool_t backlight_turn_on(backlight_t* bl)
{
    if (!bl) return tb_false;
    return backlight_set_brightness(bl, bl->max_brightness);
}

