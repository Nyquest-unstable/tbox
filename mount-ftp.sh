#!/bin/sh
# mount-ftp.sh - 挂载 FTP 到本地目录

# 配置
FTP_HOST="192.168.1.5"
FTP_USER="root"
FTP_PASS="123456"
MOUNT_POINT="./target-machine"

# 检查 curlftpfs
if ! which curlftpfs > /dev/null 2>&1; then
    echo "Error: curlftpfs not found"
    exit 1
fi

# 检查 fuse 模块
if ! lsmod | grep -q fuse; then
    echo "Loading fuse module..."
    modprobe fuse 2>/dev/null || { echo "Error: fuse not available"; exit 1; }
fi

# 创建挂载点
mkdir -p "$MOUNT_POINT"

# 卸载已存在的挂载
if mount | grep -q "$MOUNT_POINT"; then
    echo "Unmounting existing mount..."
    fusermount -u "$MOUNT_POINT" 2>/dev/null || umount "$MOUNT_POINT" 2>/dev/null
fi

# 挂载 FTP
echo "Mounting ftp://${FTP_USER}@${FTP_HOST} to ${MOUNT_POINT}..."
curlftpfs "ftp://${FTP_USER}:${FTP_PASS}@${FTP_HOST}" "$MOUNT_POINT" \
    -o uid=0,gid=0,umask=022,connect_timeout=30

# 验证
sleep 1
if mount | grep -q "$MOUNT_POINT"; then
    echo "Success: FTP mounted at ${MOUNT_POINT}"
    echo ""
    echo "Usage:"
    echo "  cd ${MOUNT_POINT}"
    echo "  ls -la"
    echo "  cp file ./"
    echo ""
    df -h "$MOUNT_POINT"
else
    echo "Error: Mount failed"
    exit 1
fi
