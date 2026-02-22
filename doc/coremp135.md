我将根据这份 M5Stack CoreMP135 产品文档为您整理一份结构清晰的 README.md 文档。

---

```markdown
# M5Stack CoreMP135

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-orange.svg)](https://www.debian.org/)
[![MCU](https://img.shields.io/badge/MCU-STM32MP135DAE7-green.svg)](https://www.st.com/)

> 基于 STM32MP135DAE7 的一体化 Linux 工控主机，适用于工业自动化、智能家居、工业物联网边缘网关等场景。

![CoreMP135](https://static-cdn.m5stack.com/resource/docs/products/core/CoreMP135/img-20250106182253647.webp)

---

## 📋 目录

- [产品概述](#产品概述)
- [硬件规格](#硬件规格)
- [接口说明](#接口说明)
- [管脚映射](#管脚映射)
- [电气参数](#电气参数)
- [机械尺寸](#机械尺寸)
- [软件支持](#软件支持)
- [快速开始](#快速开始)
- [应用场景](#应用场景)
- [参考资料](#参考资料)

---

## 产品概述

**CoreMP135** 是一款基于 **STM32MP135DAE7** 芯片的一体化 Linux 工控主机，集成单核 ARM Cortex-A7 处理器（主频 1GHz），配备 4Gbit DDR3L SDRAM 运行内存。设备预装 **Debian 12 Bookworm** 系统，开机即用，支持多种安装方式（DIN 导轨、挂墙、螺丝固定）。

### 核心特性

- 🖥️ **2.0 英寸 IPS 电容触摸屏**（320×240 分辨率，多点触控）
- 🔊 **内置 1W 扬声器**（16bit I2S 驱动）
- 🔌 **丰富工业接口**：双路千兆网口、CAN FD、RS485、多路 USB
- ⚡ **统一电源管理**：AXP2101 PMU，支持 RTC 定时唤醒/休眠
- 🔋 **灵活供电**：支持 DC 12V@2A 或 USB-C 5V@3A，可电池供电
- 🐧 **开源生态**：支持 Buildroot、Debian、UiFlow2 图形化编程

---

## 硬件规格

| 组件 | 规格 |
|:---|:---|
| **MCU** | STM32MP135DAE7 @ Arm Cortex-A7 单核 1GHz |
| **内存** | 4Gbit DDR3L SDRAM |
| **存储** | MicroSD 卡槽（预装系统） |
| **电源管理** | AXP2101 |
| **RTC** | BM8563 |
| **工作温度** | 0 ~ 40°C |
| **产品尺寸** | 81.9 × 54.0 × 39.5 mm |
| **产品重量** | 98.3g |

### 主要芯片

| 功能 | 芯片型号 |
|:---|:---|
| 485 通信 | MAX3485 |
| CAN 通信 | SIT1051T/3 ×2（高速 FDCAN） |
| USB Hub | GL852G |
| 以太网 PHY | RTL8211F ×2（最高 1Gbps） |
| 视频输出 | LT8618SXB（最高 24 位色深） |
| 屏幕驱动 | ILI9342C |
| 触摸控制 | FT6336U |
| 音频功放 | NS4168（D 类功放，8kHz~96kHz） |

---

## 接口说明

### 外部接口

| 接口类型 | 数量 | 说明 |
|:---|:---|:---|
| **千兆以太网** | 2× RJ45 | 支持 10/100/1000 Mbps |
| **USB 2.0 Type-A** | 2 | 通过 GL852G Hub 扩展 |
| **USB-C** | 1 | 支持 OTG 及 5V 供电输入 |
| **CAN FD** | 2 路 | 工业级隔离 CAN 总线 |
| **PWR485** | 1 | 9~24V 电源输入 + RS485 通信 |
| **Grove 接口** | 2 个 | PORT.A (I2C) / PORT.C (UART) |
| **Display Port** | 1 | 高清视频输出（LT8618SXB） |
| **MicroSD** | 1 | 系统存储扩展 |

### 内置外设

- **2.0" IPS LCD**：320×240 分辨率，电容多点触控
- **1W 扬声器**：2014 型腔体喇叭，8Ω
- **RTC 实时时钟**：支持定时唤醒和休眠功能

---

## 管脚映射

### M5-Bus 接口（30Pin）

| Pin | 功能 | STM32MP135 引脚 | Pin | 功能 | STM32MP135 引脚 |
|:---:|:---|:---|:---:|:---|:---|
| 1 | GND | - | 2 | GND | - |
| 3 | GPIO | PA0 | 4 | PB_IN | PA3 |
| 5 | AXP-PWR-OK | - | 6 | GND | - |
| 7 | GPIO/SPI4_MO | PE11/PB13 | 8 | SPI4_MI | PE13 |
| 9 | PB_OUT | PE9 | 10 | SPI4_SCK | PB4 |
| 11 | 3V3 | - | 12 | U2RX | PH8 |
| 13 | U2TX | PF11 | 14 | 3V3 | - |
| 15 | DS-USB1-N | - | 16 | DS-USB1-P | - |
| 17 | I2C1-SDA | PG8 | 18 | I2C1-SCL | PB8 |
| 19 | I2C2-SDA | PG9 | 20 | I2C2-SCL | PF2 |
| 21 | GPIO | PA6 | 22 | GPIO | PB10 |
| 23 | GPIO | PA5 | 24 | GPIO | PC13 |
| 25 | NC | PA1 | 26 | NC | - |
| 27 | 5V | - | 28 | NC | - |
| 29 | BAT | - | 30 | - | - |

### Linux 设备节点映射

| 设备 | 节点路径 | 对应硬件 |
|:---|:---|:---|
| USART2 | `/dev/ttySTM2` | M5-Bus UART |
| USART3 | `/dev/ttySTM3` | RS485 (MAX3485) |
| USART6 | `/dev/ttySTM0` | PORT.C Grove |
| I2C1 | `/dev/i2c-2` | M5-Bus I2C |
| I2C2 | `/dev/i2c-3` | M5-Bus I2C |
| I2C3 | `/dev/i2c-0` | 内部外设（触摸、RTC、音频）|
| I2C5 | `/dev/i2c-1` | PORT.A Grove |

### 专用接口映射

**CAN FD**
- FDCAN1_TX: PE3 | FDCAN1_RX: PE10
- FDCAN2_TX: PG0 | FDCAN2_RX: PE0

**Display (LT8618SXB)**
- MCLK: PF13 | SCLK: PF8 | SDO: PA3 | WS: PG10
- I2C3_SDA: PH7 | I2C3_SCL: PH12

**Audio (NS4168)**
- LRCLK: PE4 | BCLK: PA4 | SDATA: PD6

**MicroSD**
- SD_D0: PC8 | SD_D1: PC9 | SD_D2: PC10 | SD_D3: PC11
- SD_CMD: PD2 | SD_CLK: PC12

**Screen & Touch**
- LCD_RST: PI0 | LCD_MOSI: PC0 | LCD_SCK: PH5 | LCD_CS: PH4
- TP_SCL: PH12 (I2C3) | TP_SDA: PH7 (I2C3)

---

## 电气参数

| 参数 | 规格 |
|:---|:---|
| **直流电源输入** | DC 12V @ 2A |
| **USB-C 供电** | 5V @ 3A |
| **PWR485 输入范围** | 9 ~ 24V DC |
| **工作温度** | 0°C ~ 40°C |
| **存储温度** | -20°C ~ 70°C |

### M5-Bus 电源控制

通过 GPIO 控制总线 5V 输入/输出模式：

```bash
# 导出 GPIO
echo 131 > /sys/class/gpio/export

# 设置为输出模式
echo out > /sys/class/gpio/PI3/direction

# 开启向下输出（高电平）
echo 1 > /sys/class/gpio/PI3/value

# 关闭输出（低电平，输入模式）
echo 0 > /sys/class/gpio/PI3/value
```

> **原理**：BUS_OUT_EN 低电平 = 5V 输入模式，高电平 = 5V 输出模式

---

## 机械尺寸

- **外形尺寸**：81.9 × 54.0 × 39.5 mm
- **安装方式**：DIN 导轨 / 挂墙 / M3 螺丝固定
- **包装尺寸**：122.2 × 71.5 × 61.6 mm
- **净重**：98.3g | **毛重**：154.7g

![尺寸图](https://static-cdn.m5stack.com/resource/docs/products/core/CoreMP135/img-20250106182326599.webp)

---

## 软件支持

### 操作系统

| 镜像版本 | 内核版本 | 类型 |
|:---|:---|:---|
| M5CoreMP135_debian12_20240919 | 5.15.118 | Debian 12 (推荐) |
| M5CoreMP135_buildroot_20240628 | 5.15.118 | Buildroot (轻量) |

### 开发框架

- **UiFlow2**：图形化编程平台，支持积木式开发和 Python 代码生成
- **M5Stack Linux Libs**：官方 C++/Python 硬件控制库
- **Buildroot External**：自定义根文件系统构建

### 开发资源

- [镜像烧录与软件更新指南](https://docs.m5stack.com/zh_CN/core/CoreMP135)
- [应用开发框架文档](https://docs.m5stack.com/zh_CN/core/CoreMP135)
- [Buildroot 编译指南](https://docs.m5stack.com/zh_CN/core/CoreMP135)
- [设备树覆盖(DTB)说明](https://docs.m5stack.com/zh_CN/core/CoreMP135)

---

## 快速开始

### 1. 硬件准备

- CoreMP135 主机（含预装系统的 MicroSD 卡）
- DC 12V/2A 电源适配器 或 USB-C 5V/3A 电源
- （可选）网线、USB 外设、HDMI 显示器

### 2. 首次启动

1. 插入 MicroSD 卡（如未预装）
2. 连接电源（DC 12V 或 USB-C）
3. 按下侧面电源按钮开机（按住 6 秒关机）
4. 等待系统启动（约 30-60 秒）

### 3. 连接方式

- **串口调试**：USB-C 虚拟串口 / UART2
- **SSH 远程**：通过双网口连接后 SSH 登录
- **HDMI 显示**：连接外接显示器和 USB 键鼠

### 4. 基础配置

```bash
# 查看系统信息
uname -a
cat /etc/os-release

# 查看设备节点
ls /dev/ttySTM*  # 串口
ls /dev/i2c-*    # I2C 总线
ls /dev/spi*     # SPI 总线

# 网络配置（Debian）
sudo nmtui
```

---

## 应用场景

- 🏭 **工业自动化**：PLC 通信、设备控制、数据采集
- 🏠 **智能家居**：中央控制网关、触控面板
- 🌐 **工业物联网边缘网关**：协议转换、边缘计算、数据上云
- 🤖 **机器人运动控制中枢**：CAN 总线控制、实时运动规划
- 🎓 **教育和开发**：嵌入式 Linux 学习、工业协议开发

---

## 包装内容

- [x] CoreMP135 主机 ×1
- [x] M3 六角扳手 ×1
- [x] HT3.96-4P 接线端子 ×1
- [x] 2.54mm-2P 接线端子（绿色）×2
- [x] MicroSD 卡（已预装系统）×1
- [x] 说明书 ×1

---

## 参考资料

### 官方文档

- [CoreMP135 官方文档](https://docs.m5stack.com/zh_CN/core/CoreMP135)
- [STM32MP135 参考手册](https://www.st.com/zh/microcontrollers-microprocessors/stm32mp135.html)
- [M5Stack GitHub](https://github.com/m5stack)

### 数据手册

| 芯片 | 文档 |
|:---|:---|
| STM32MP135DAE7 | [Datasheet](https://www.st.com/resource/en/datasheet/stm32mp135.pdf) |
| GL852G (USB Hub) | [Datasheet](https://www.genesyslogic.com.tw/products/gl852g.htm) |
| LT8618SXB (HDMI) | [Datasheet](https://www.lontiumsemi.com/) |
| RTL8211F (Ethernet) | [Datasheet](https://www.realtek.com/) |
| FT6336U (Touch) | [Datasheet](https://www.focaltech-systems.com/) |
| NS4168 (Audio Amp) | [Datasheet](https://www.nsiway.com.cn/) |
| BM8563 (RTC) | [Datasheet](https://www.belling.com.cn/) |
| AXP2101 (PMU) | [Datasheet](https://www.x-powers.com/) |

### 社区与支持

- 🌐 官方网站：https://m5stack.com
- 💬 社区论坛：https://community.m5stack.com
- 🐛 问题反馈：https://github.com/m5stack/CoreMP135/issues

---

## 许可协议

本项目硬件设计遵循开源硬件协议，软件部分遵循 GPL/MIT 等相应开源协议。详细许可信息请参考各组件的 LICENSE 文件。

---

**© 2024-2026 M5Stack Technology Co., Ltd.**
```

---

这份 README.md 文档涵盖了产品的核心信息，包括：

1. **产品概述** - 核心特性和应用场景
2. **详细规格** - 硬件参数和主要芯片
3. **接口说明** - 所有外部接口和内部外设
4. **管脚映射** - M5-Bus 详细引脚定义和 Linux 设备节点对应关系
5. **电气参数** - 供电规格和 GPIO 电源控制方法
6. **机械尺寸** - 物理规格和安装方式
7. **软件支持** - 操作系统、开发框架和资源链接
8. **快速开始** - 上手指南和基础命令
9. **参考资料** - 官方文档和数据手册链接