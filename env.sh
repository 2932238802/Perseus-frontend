#!/bin/bash
set -e 

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}========================================================${NC}"
echo -e "${BLUE}        🚀 欢迎配置 Perseus IDE 开发环境 🚀           ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo -e "${YELLOW}由于需要安装系统依赖，稍后可能会要求您输入 sudo 密码${NC}\n"

echo -e "${GREEN}[1/5] 正在更新系统软件包列表...${NC}"
sudo apt-get update -y

echo -e "${GREEN}[2/5] 正在安装 C++ 核心编译工具链 (CMake, Ninja, GCC, Clang)...${NC}"
sudo apt-get install -y build-essential cmake ninja-build gdb git clang

echo -e "${GREEN}[3/5] 正在安装 Qt 6 框架及底层图形库依赖...${NC}"
sudo apt-get install -y qt6-base-dev qt6-base-dev-tools \
    libgl1-mesa-dev libxcb-cursor0 \
    libxkbcommon-x11-0 libxkbcommon-dev

echo -e "${GREEN}[4/5] 正在安装 Qt 6 WebEngine 与 WebChannel (终端与前端 UI 依赖)...${NC}"
sudo apt-get install -y qt6-webengine-dev qt6-webchannel-dev

echo -e "${GREEN}[5/5] 正在安装 Clangd 代码补全与格式化引擎...${NC}"
sudo apt-get install -y clangd clang-format

echo -e "\n${BLUE}========================================================${NC}"
echo -e "${GREEN} 环境安装全部完成！${NC}"
echo -e "${BLUE}========================================================${NC}"

echo -n "CMake 版本: "
cmake --version | head -n 1 || echo -e "${RED}未找到 CMake${NC}"

echo -n "Qt6 版本: "
qmake6 -v | grep "Qt version" || echo -e "${RED}未找到 Qt6${NC}"

echo -n "Clang 编译器: "
clang++ --version | head -n 1 || echo -e "${RED}未找到 Clang${NC}"

echo -n "Clangd 版本: "
clangd --version || echo -e "${RED}未找到 Clangd${NC}"

echo -e "\n${YELLOW}💡 接下来您可以使用 ./re-build.sh 脚本来重新构建项目 ${NC}"
