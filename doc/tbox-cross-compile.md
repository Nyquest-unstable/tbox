# TBOX 交叉编译与部署技能

## 项目概述

TBOX 是一个用 C 语言实现的跨平台开发库，支持 Windows、MacOS、Linux、Android、iOS、*BSD 等平台。

## 重要平台说明

**关键信息：** 当前项目的目标平台(target)是ARM架构的32位平台，但编译宿主机(host)是x86_64平台。
因此，所有编译产出的二进制文件都是针对ARM 32位架构的，不能在x86_64编译主机上直接运行。
请勿在编译完成后尝试直接运行生成的可执行文件，因为它们不兼容当前编译主机架构。

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
# 配置交叉编译，目标平台为ARM 32位
xmake f -p linux -a arm --toolchain=myarm --lvgl=y

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

## 关键提醒

由于目标平台是ARM 32位架构，而编译宿主机是x86_64架构，编译产出的所有二进制文件都无法在当前编译主机上直接运行。
请勿尝试在编译完成后直接运行生成的可执行文件，必须将其部署到ARM 32位目标平台上运行。

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
2. 重新编译项目（针对ARM 32位平台）
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
├── skills/                # 技能文档目录
│   ├── create-tbox-audio-app.md   # 音频应用开发技能
│   ├── create-tbox-hello-world.md # Hello World 应用开发技能
│   └── tbox-cross-compile.md      # 交叉编译技能（当前文件）
├── src/                   # 源码目录
│   ├── tbox/              # TBOX 库源码
│   ├── demo/              # 示例应用
│   └── hello/             # Hello World 应用示例
└── out/                   # 编译输出软链接
```