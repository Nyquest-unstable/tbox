
# TBOX 项目中创建 Hello World 应用技能

## 概述

本技能说明如何在 TBOX 项目中创建一个简单的 Hello World 应用程序。

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

## 编译和运行

### 编译应用

```bash
xmake build hello
```

### 运行应用

```bash
xmake run hello
```

## 编译输出

- **实际编译文件**：`build/linux/{arch}/release/hello`
- **软链接**：`out/hello`（方便访问）

## 切换架构编译

如果需要切换架构（如从 arm 切换到 x86_64）：

```bash
# 切换到 x86_64 架构
xmake f -a x86_64

# 重新编译
xmake build hello

# 运行
xmake run hello
```

## 清理构建

```bash
xmake c -a
```

## 常用命令总结

| 命令 | 说明 |
|------|------|
| `xmake build hello` | 编译 hello 应用 |
| `xmake run hello` | 运行 hello 应用 |
| `xmake f -a {arch}` | 配置目标架构 |
| `xmake c -a` | 清理所有构建文件 |
| `xmake show` | 查看当前项目配置 |

## 注意事项

1. **目录结构**：保持应用在 `src/` 目录下，与 `demo`、`tbox` 同级
2. **构建配置**：每个应用需要自己的 `xmake.lua`
3. **依赖关系**：使用 `add_deps("tbox")` 确保 TBOX 库被正确链接
4. **初始化与清理**：使用 `tb_init()` 初始化 TBOX，`tb_exit()` 清理资源
5. **输出函数**：使用 `tb_printf()` 替代标准 C 的 `printf()`
