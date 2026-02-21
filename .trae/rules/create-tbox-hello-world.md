# TBOX 项目中创建 Hello World 应用技能

## 概述

本技能说明如何在 TBOX 项目中创建一个简单的 Hello World 应用程序。

## 重要平台说明

**关键信息：** 当前项目的目标平台(target)是ARM架构的32位平台，但编译宿主机(host)是x86_64平台。
因此，所有编译产出的二进制文件都是针对ARM 32位架构的，不能在x86_64编译主机上直接运行。
请勿在编译完成后尝试直接运行生成的可执行文件，因为它们不兼容当前编译主机架构。

## 项目结构

在 TBOX 项目中，所有应用都放在 `src/` 目录下，保持统一的项目结构：

```
tbox/
├── src/
│   ├── tbox/          # TBOX 库源码
│   ├── demo/          # 示例应用
│   └── hello/         # 新创建的 Hello World 应用
│       ├── hello.c    # 应用源代码
│       └── xmake.lua  # 应用构建配置
├── xmake.lua          # 主项目配置
└── out/               # 编译输出软链接
```

## 创建步骤

### 1. 创建应用目录

在 `src/` 目录下创建应用文件夹：

```bash
mkdir -p src/hello
```

### 2. 编写应用源代码

创建 `src/hello/hello.c`，使用 TBOX 库：

```c
#include "tbox/tbox.h"

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    if (!tb_init(tb_null, tb_null))
        return -1;

    tb_printf("Hello World!\n");

    tb_exit();
    return 0;
}
```

### 3. 编写 xmake.lua 构建配置

创建 `src/hello/xmake.lua`：

```lua
target("hello")

    add_deps("tbox")
    set_kind("binary")
    add_files("hello.c")

    after_build(function (target)
        import("lib.detect.find_tool")
        local ln = find_tool("ln")
        if ln then
            local outdir = path.join(os.projectdir(), "out")
            os.mkdir(outdir)
            local targetfile = path.absolute(target:targetfile())
            local linkfile = path.join(outdir, path.filename(targetfile))
            os.tryrm(linkfile)
            os.vrunv(ln.program, {"-sf", targetfile, linkfile})
            print("已软链接到: " .. linkfile)
        end
    end)
```

### 4. 更新主项目配置

在 `src/xmake.lua` 中添加对新应用的引用：

```lua
-- include project directories
includes(format("tbox/%s.lua", (has_config("micro") and "micro" or "xmake")))
if has_config("demo") then
    includes(format("demo/%s.lua", (has_config("micro") and "micro" or "xmake")))
end
includes("hello/xmake.lua")
```

## 交叉编译

### 配置 ARM 32位平台交叉编译环境

```bash
# 配置交叉编译环境，目标平台为ARM 32位
xmake f -p linux -a arm --toolchain=myarm

# 或者使用更具体的参数
xmake f -p cross -a arm -m release --sdk=/path/to/arm/toolchain
```

### 编译应用

```bash
xmake build hello
```

## 部署到目标平台

由于编译产出的是ARM 32位架构的二进制文件，需要将其部署到ARM 32位平台上才能运行：

1. 将编译产出的可执行文件复制到ARM 32位目标设备
2. 确保目标设备有必要的依赖库
3. 在目标设备上运行应用