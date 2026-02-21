#!/bin/bash
# deploy.sh - 编译并部署到目标机器

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR="$PROJECT_DIR/target-machine"
OUT_DIR="$PROJECT_DIR/out"
APPS_DIR="$TARGET_DIR/apps"

echo "========================================="
echo "  TBOX 部署脚本"
echo "========================================="
echo ""

# 检查 target-machine 是否挂载
if ! mount | grep -q "$TARGET_DIR"; then
    echo "错误: target-machine 未挂载"
    echo "请先运行: ./mount-ftp.sh"
    exit 1
fi

echo "✓ target-machine 已挂载"
echo ""

# 编译
echo "正在编译..."
cd "$PROJECT_DIR"
xmake -r
echo ""
echo "✓ 编译成功"
echo ""

# 创建 apps 目录
echo "正在创建 apps 目录..."
mkdir -p "$APPS_DIR"
echo "✓ apps 目录已创建"
echo ""

# 复制 out 目录下的所有文件（解析符号链接）到 apps 目录
echo "正在部署文件到 apps 目录..."
if [ -d "$OUT_DIR" ]; then
    # 遍历 out 目录下的所有文件
    for file in "$OUT_DIR"/*; do
        if [ -e "$file" ]; then
            filename=$(basename "$file")
            # 如果是符号链接，复制实际文件；否则直接复制
            if [ -L "$file" ]; then
                real_file=$(readlink -f "$file")
                if [ -e "$real_file" ]; then
                    cp -f "$real_file" "$APPS_DIR/$filename"
                    echo "  复制 (解析链接): $filename"
                fi
            else
                cp -f "$file" "$APPS_DIR/$filename"
                echo "  复制: $filename"
            fi
        fi
    done
    echo "✓ 文件已部署到目标机器的 apps 目录"
else
    echo "警告: out 目录不存在"
fi
echo ""

# 设置可执行权限
echo "正在设置可执行权限..."
find "$APPS_DIR" -type f -exec chmod +x {} \; 2>/dev/null || true
echo "✓ 可执行权限已设置"
echo ""

echo "========================================="
echo "  部署完成！"
echo "========================================="
echo ""
echo "目标机器 apps 目录内容:"
ls -lh "$APPS_DIR/"
echo ""
echo "接下来请登录目标机器运行:"
echo "  ssh root@192.168.1.5"
echo "  cd /root/apps"
echo "  ./demo"
echo ""
