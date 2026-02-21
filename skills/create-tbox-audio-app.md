# TBOX 音频应用开发技能

## 概述

本技能说明如何在 TBOX 项目中创建一个音频处理应用程序。

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
                tb_trace_i("Audio file size: %llu bytes", file_size);

                if (verbose) {
                    tb_byte_t header[128];
                    tb_long_t read = tb_stream_read(stream, header, sizeof(header));
                    if (read > 0) {
                        tb_trace_i("First %ld bytes of file:", read);
                        tb_dump_data(header, read);
                    }
                }
                
                tb_stream_clos(stream);
            } else {
                tb_trace_e("Could not open audio file: %s", audio_file);
            }
            tb_stream_exit(stream);
        } else {
            tb_trace_e("Could not create stream for: %s", audio_file);
        }
    } else {
        tb_trace_i("No audio file specified. This is a demonstration of audio processing concepts.");
        tb_trace_i("");
        tb_trace_i("Supported operations:");
        tb_trace_i("- File I/O using TBOX streams");
        tb_trace_i("- Memory management for audio buffers");
        tb_trace_i("- Platform abstraction for audio APIs");
        tb_trace_i("- Data processing utilities");
    }

    tb_trace_i("TBOX Audio Application Finished");
    
    // Exit TBOX
    tb_exit();
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

在 `src/xmake.lua` 中添加对音频应用的引用：

```lua
includes("audio/xmake.lua")
```

## 编译和运行

### 编译应用

```bash
xmake build audio
```

### 运行应用

```bash
xmake run audio
```

或指定音频文件：

```bash
xmake run audio -f sample.wav -v
```

## 编译输出

- **实际编译文件**：`build/linux/{arch}/release/audio`
- **软链接**：`out/audio`（方便访问）

## 音频处理功能

该音频应用演示了以下功能：

1. **文件 I/O**：使用 TBOX 流处理音频文件
2. **数据读取**：从音频文件中读取原始数据
3. **十六进制转储**：以十六进制格式显示音频文件头部
4. **命令行参数解析**：支持多种音频处理选项
5. **平台抽象**：利用 TBOX 的跨平台能力

## 扩展建议

可以进一步扩展此音频应用以支持：

- 音频格式转换
- 音频数据解码/编码
- 音频滤波和效果处理
- 实时音频流处理
- 音频设备 I/O（需要额外的平台特定代码）

## 常用命令总结

| 命令 | 说明 |
|------|------|
| `xmake build audio` | 编译音频应用 |
| `xmake run audio` | 运行音频应用 |
| `xmake run audio -h` | 显示帮助信息 |
| `xmake run audio -f file.wav -v` | 详细模式处理音频文件 |
| `xmake c -a` | 清理所有构建文件 |