# TBOX 音频应用开发技能

## 概述

本技能说明如何在 TBOX 项目中创建一个音频处理应用程序。

## 重要平台说明

**关键信息：** 当前项目的目标平台(target)是ARM架构的32位平台，但编译宿主机(host)是x86_64平台。
因此，所有编译产出的二进制文件都是针对ARM 32位架构的，不能在x86_64编译主机上直接运行。
请勿在编译完成后尝试直接运行生成的可执行文件，因为它们不兼容当前编译主机架构。

## 项目结构

在 TBOX 项目中，音频应用放在 `src/` 目录下，保持统一的项目结构：

```
tbox/
├── src/
│   ├── tbox/          # TBOX 库源码
│   ├── demo/          # 示例应用
│   ├── hello/         # Hello World 应用
│   ├── ui/            # UI 应用
│   └── audio/         # 新创建的音频应用
│       ├── audio.c    # 应用源代码
│       └── xmake.lua  # 应用构建配置
├── xmake.lua          # 主项目配置
└── out/               # 编译输出软链接
```

## 创建步骤

### 1. 创建应用目录

在 `src/` 目录下创建应用文件夹：

```bash
mkdir -p src/audio
```

### 2. 编写应用源代码

创建 `src/audio/audio.c`，使用 TBOX 库进行音频处理：

```c
#include "tbox/tbox.h"

// A simple audio application using TBOX
// This example shows how to use TBOX for basic audio processing tasks

static tb_void_t print_usage(tb_char_t const* program)
{
    tb_trace_i("usage: %s [options]", program);
    tb_trace_i("options:");
    tb_trace_i("  -h, --help     Show this help message");
    tb_trace_i("  -f, --file     Audio file to process");
    tb_trace_i("  -v, --verbose  Verbose output");
    tb_trace_i("");
    tb_trace_i("examples:");
    tb_trace_i("  %s -f sample.wav", program);
}

tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // Initialize TBOX
    if (!tb_init(tb_null, tb_null)) {
        tb_trace_e("Failed to initialize TBOX!");
        return -1;
    }

    tb_bool_t verbose = tb_false;
    tb_char_t const* audio_file = tb_null;

    // Parse command line arguments
    for (tb_int_t i = 1; i < argc; i++) {
        if (tb_strcmp(argv[i], "-h") == 0 || tb_strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (tb_strcmp(argv[i], "-v") == 0 || tb_strcmp(argv[i], "--verbose") == 0) {
            verbose = tb_true;
        }
        else if ((tb_strcmp(argv[i], "-f") == 0 || tb_strcmp(argv[i], "--file") == 0) && i + 1 < argc) {
            audio_file = argv[++i];
        }
        else {
            tb_trace_e("Unknown option: %s", argv[i]);
            print_usage(argv[0]);
            return -1;
        }
    }

    tb_trace_i("TBOX Audio Application Started");
    
    if (verbose) {
        tb_trace_i("Verbose mode enabled");
    }

    if (audio_file) {
        tb_trace_i("Processing audio file: %s", audio_file);
        
        // Example of using TBOX stream to read an audio file
        tb_stream_ref_t stream = tb_stream_init_from_url(audio_file);
        if (stream) {
            if (tb_stream_open(stream)) {
                tb_hize_t file_size = tb_stream_size(stream);
                tb_trace_i("File size: %llu bytes", file_size);
            }
            tb_stream_exit(stream);
        } else {
            tb_trace_e("Could not open audio file: %s", audio_file);
        }
    } else {
        tb_trace_i("No audio file specified. Use -f option to specify a file.");
    }

    tb_trace_i("TBOX Audio Application Finished");
    
    return 0;
}
```

### 3. 编写 xmake.lua 构建配置

创建 `src/audio/xmake.lua`：

```lua
target("audio")

    add_deps("tbox")
    set_kind("binary")
    add_files("audio.c")

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
includes("audio/xmake.lua")
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
xmake build audio
```

## 部署到目标平台

由于编译产出的是ARM 32位架构的二进制文件，需要将其部署到ARM 32位平台上才能运行：

1. 将编译产出的可执行文件复制到ARM 32位目标设备
2. 确保目标设备有必要的依赖库
3. 在目标设备上运行应用