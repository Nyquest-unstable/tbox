# TBOX 交叉编译与部署技能

## 项目概述

TBOX 是一个用 C 语言实现的跨平台开发库，支持 Windows、MacOS、Linux、Android、iOS、*BSD 等平台。

## 交叉编译配置

### 1. 自定义工具链配置

在项目根目录的 `xmake.lua` 中添加自定义工具链：

```lua
toolchain("myarm")
    set_kind("cross")
    set_sdkdir("/path/to/sdk")
    set_cross("arm-buildroot-linux-gnueabihf-")
toolchain_end()
```

### 2. 配置并编译

```bash
# 配置交叉编译
xmake f -p linux -a arm --toolchain=myarm

# 编译
xmake

# 强制重新编译
xmake -r
```

## 输出位置

- **实际编译输出**：`build/linux/arm/release/`
- **软链接（方便访问）**：`out/`

包含文件：
- `libtbox.a` - 静态库
- `demo` - 可执行文件

## 目标机器部署

### 1. FTP 挂载

使用 `mount-ftp.sh` 脚本将目标机器通过 FTP 挂载到本地：

```bash
./mount-ftp.sh
```

配置信息：
- FTP 主机：192.168.1.5
- 用户：root
- 密码：123456
- 挂载点：`./target-machine`

### 2. 一键部署

使用 `deploy.sh` 脚本一键编译并部署：

```bash
./deploy.sh
```

这个脚本会：
1. 检查 target-machine 是否已挂载
2. 重新编译项目
3. 将编译产物复制到目标机器

### 3. 手动复制文件

```bash
cp out/demo out/libtbox.a target-machine/
```

## 在目标机器上运行

SSH 登录到目标机器：

```bash
ssh root@192.168.1.5
```

然后运行：

```bash
cd /root
chmod +x demo
./demo
```

## 项目结构

```
tbox/
├── xmake.lua              # 主构建配置（含自定义工具链）
├── mount-ftp.sh           # FTP 挂载脚本
├── deploy.sh              # 一键部署脚本
├── src/                   # 源代码
├── out/                   # 软链接输出
├── build/linux/arm/release/  # 实际编译输出
└── target-machine/        # 挂载的目标机器文件系统
    ├── demo
    └── libtbox.a
```

## 常用编译模式

```bash
# 调试模式
xmake f -p linux -a arm --toolchain=myarm -m debug
xmake

# 最小化模式（适合嵌入式）
xmake f -p linux -a arm --toolchain=myarm --small=y
xmake

# 微型模式（仅微内核，约64K）
xmake f -p linux -a arm --toolchain=myarm --micro=y
xmake
```

## 清理构建

```bash
# 清理所有配置和编译文件
xmake c -a
```

## 验证文件架构

使用 `file` 命令验证编译产物的架构：

```bash
file build/linux/arm/release/demo
file build/linux/arm/release/libtbox.a
```

预期输出（ARM 32位）：
```
demo: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV)
```

## 后处理配置

在 `src/tbox/xmake.lua` 和 `src/demo/xmake.lua` 中添加后处理钩子，自动创建软链接：

```lua
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

## 日常开发工作流

1. 修改源代码
2. 运行 `./deploy.sh` 一键编译部署
3. SSH 登录目标机器测试
4. 重复以上步骤

## 参考资料

- [TBOX 在线文档](https://docs.tboox.top)
- [TBOX GitHub](https://github.com/tboox/tbox)
- [XMake 构建工具](https://github.com/xmake-io/xmake)
