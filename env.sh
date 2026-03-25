#!/bin/bash
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'
echo -e "${BLUE}========================================================${NC}"
echo -e "${BLUE}        🚀 欢迎配置 Perseus IDE 开发环境 🚀           ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo -e "${YELLOW}由于需要安装系统依赖，稍后可能会要求您输入 sudo 密码${NC}\n"
echo -e "${GREEN}[1/4] 正在更新系统软件包列表...${NC}"
sudo apt-get update -y
echo -e "${GREEN}[2/4] 正在安装 C++ 核心编译工具链 (CMake, Ninja, GCC)...${NC}"
sudo apt-get install -y build-essential cmake ninja-build gdb git
echo -e "${GREEN}[3/4] 正在安装 Qt 6 框架及底层图形库依赖...${NC}"
sudo apt-get install -y qt6-base-dev qt6-base-dev-tools libgl1-mesa-dev libxcb-cursor0 libxkbcommon-x11-0
echo -e "${GREEN}[4/4] 正在安装 Clangd 代码补全与诊断引擎...${NC}"
sudo apt-get install -y clangd
echo -e "\n${BLUE}========================================================${NC}"
echo -e "${GREEN}🎉 环境安装全部完成！正在为您校验版本：${NC}"
echo -e "${BLUE}========================================================${NC}"
echo -n "CMake 版本: "
cmake --version | head -n 1
echo -n "Qt6 版本: "
qmake6 -v | grep "Qt version"
echo -n "Clangd 版本: "
clangd --version
echo -e "\n${YELLOW}💡 接下来您可以使用运行 re-build.sh 脚本来构建项目 ${NC}"