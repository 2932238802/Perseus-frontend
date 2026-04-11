#!/bin/bash
set -e

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}========================================================${NC}"
echo -e "${BLUE}        Perseus IDE — 开发环境一键配置              ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo -e "${YELLOW}由于需要安装系统依赖，稍后可能会要求您输入 sudo 密码${NC}\n"

echo -e "${GREEN}[1/6] 正在更新系统软件包列表...${NC}"
sudo apt-get update -y

echo -e "${GREEN}[2/6] 正在安装 C++ 核心编译工具链...${NC}"
sudo apt-get install -y \
    build-essential cmake ninja-build gdb git \
    clang-18

echo -e "${GREEN}[3/6] 正在安装 Qt 6 基础框架及图形库依赖...${NC}"
sudo apt-get install -y \
    qt6-base-dev qt6-base-dev-tools \
    libgl1-mesa-dev libxcb-cursor0 \
    libxkbcommon-x11-0 libxkbcommon-dev

echo -e "${GREEN}[4/6] 正在安装 Qt 6 WebEngine 与 WebChannel...${NC}"
sudo apt-get install -y \
    qt6-webengine-dev qt6-webchannel-dev

echo -e "${GREEN}[5/6] 正在安装 Clangd 与 Clang-Format...${NC}"
sudo apt-get install -y clangd clang-format

echo -e "${GREEN}[6/6] 正在安装 WSL2 GUI 支持库 (D-Bus / X11)...${NC}"
sudo apt-get install -y dbus-x11 x11-apps 2>/dev/null || true

echo -e "\n${BLUE}========================================================${NC}"
echo -e "${GREEN} ✅ 环境安装完成！以下是版本验证：${NC}"
echo -e "${BLUE}========================================================${NC}\n"

check_tool() {
    local name="$1"
    local cmd="$2"
    printf "  %-20s" "$name"
    if eval "$cmd" 2>/dev/null; then
        echo ""
    else
        echo -e "${RED}未找到${NC}"
    fi
}

check_tool "CMake"          "cmake --version | head -n 1"
check_tool "Ninja"          "ninja --version"
check_tool "Clang-18"       "clang-18 --version | head -n 1"
check_tool "GDB"            "gdb --version | head -n 1"
check_tool "Clangd"         "clangd --version | head -n 1"
check_tool "Clang-Format"   "clang-format --version"
check_tool "Qt6 (qmake6)"   "qmake6 --version 2>/dev/null | tail -n 1"

echo -e "\n${BLUE}========================================================${NC}"
echo -e "${YELLOW}接下来：${NC}"
echo -e "   ${GREEN}./re-build.sh${NC}    首次完整构建（推荐）"
echo -e "   ${GREEN}./build.sh${NC}       增量构建 + 运行"
echo -e "   ${GREEN}make run${NC}         仅运行已编译的程序"
echo -e "${BLUE}========================================================${NC}"
