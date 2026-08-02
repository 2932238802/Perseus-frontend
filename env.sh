#!/usr/bin/env bash
# Copyright (c) 2026 LosAngelous (shengjie.lin)

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}"
cd "${PROJECT_ROOT}"

set -euo pipefail

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${BLUE}========================================================${NC}"
echo -e "${BLUE}       Perseus IDE - 开发环境一键配置                  ${NC}"
echo -e "${BLUE}========================================================${NC}"
echo -e "${YELLOW}安装系统依赖时可能要求输入 sudo 密码${NC}"
echo

echo -e "${GREEN}[1/8] 正在更新系统软件包列表...${NC}"
sudo apt-get update

echo -e "${GREEN}[2/8] 正在安装 C++ 编译工具链...${NC}"
sudo apt-get install -y \
    build-essential \
    gcc \
    g++ \
    cmake \
    ninja-build \
    gdb \
    git \
    pkg-config \
    clang \
    clangd \
    clang-format \
    ca-certificates

echo -e "${GREEN}[3/8] 正在安装 libgit2 构建依赖...${NC}"
sudo apt-get install -y \
    zlib1g-dev \
    libssl-dev

echo -e "${GREEN}[4/8] 正在安装 Qt5 开发环境...${NC}"
sudo apt-get install -y \
    qtbase5-dev \
    qtbase5-dev-tools \
    qtchooser \
    qt5-qmake \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libxcb-cursor0 \
    libxcb-xinerama0 \
    libxcb-keysyms1 \
    libxcb-image0 \
    libxcb-render-util0 \
    libxkbcommon-x11-0 \
    libxkbcommon-dev

echo -e "${GREEN}[5/8] 正在安装 Qt6 开发环境...${NC}"
sudo apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    libqt6core5compat6-dev \
    qt6-webengine-dev \
    qt6-webchannel-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libxcb-cursor0 \
    libxcb-xinerama0 \
    libxcb-keysyms1 \
    libxcb-image0 \
    libxcb-render-util0 \
    libxkbcommon-x11-0 \
    libxkbcommon-dev

echo -e "${GREEN}[6/8] 正在安装测试及 WSL2 GUI 支持工具...${NC}"
sudo apt-get install -y \
    xvfb \
    dbus-x11 \
    x11-apps


echo -e "${GREEN}[7/8] 正在准备第三方依赖源码...${NC}"

mkdir -p "${PROJECT_ROOT}/third_party"

# 从 .gitmodules 读取全部子模块路径
mapfile -t SUBMODULE_PATHS < <(git -C "${PROJECT_ROOT}" config -f "${PROJECT_ROOT}/.gitmodules" \
    --get-regexp '^submodule\..*\.path$' 2>/dev/null | awk '{print $2}')

prepare_submodule() {
    local sub_path="$1"
    local sub_dir="${PROJECT_ROOT}/${sub_path}"
    # 子模块名: third_party/libgit2 -> LIBGIT2, third_party/tree-sitter -> TREE_SITTER
    local sub_key
    sub_key="$(basename "${sub_path}" | tr '[:lower:]-' '[:upper:]_')"

    # 支持环境变量覆盖: LIBGIT2_REF / LIBGIT2_URL / TREE_SITTER_REF ...
    local sub_ref="${sub_key}_REF"
    local sub_url="${sub_key}_URL"
    local ref_value="${!sub_ref:-}"
    local url_value="${!sub_url:-}"

    if [ -z "${url_value}" ]; then
        url_value="$(git -C "${PROJECT_ROOT}" config -f "${PROJECT_ROOT}/.gitmodules" \
            --get "submodule.${sub_path}.url" 2>/dev/null || true)"
    fi

    if git -C "${PROJECT_ROOT}" ls-files --stage -- "${sub_path}" |
        grep -q '^160000 '; then

        echo -e "${BLUE}检测到 ${sub_path} Git 子模块，正在初始化...${NC}"

        git -C "${PROJECT_ROOT}" submodule update \
            --init \
            --recursive \
            -- "${sub_path}"

    elif [ -f "${sub_dir}/CMakeLists.txt" ]; then

        echo -e "${BLUE}检测到已有 ${sub_path} 源码，跳过下载${NC}"

    elif [ -e "${sub_dir}" ]; then

        echo -e "${RED}错误：${NC}"
        echo -e "  ${sub_dir} 已存在，但不是有效的源码目录。"
        echo -e "  请清理该目录后重新执行，或者手动放入源码。"
        exit 1

    else

        if [ -z "${url_value}" ]; then
            echo -e "${RED}错误：${sub_path} 缺少下载地址（.gitmodules 未配置）${NC}"
            exit 1
        fi

        echo -e "${BLUE}未检测到 ${sub_path} 子模块，正在下载...${NC}"

        if [ -n "${ref_value}" ]; then
            git clone \
                --depth 1 \
                --branch "${ref_value}" \
                "${url_value}" \
                "${sub_dir}"
        else
            git clone \
                --depth 1 \
                "${url_value}" \
                "${sub_dir}"
        fi
    fi

    if [ ! -f "${sub_dir}/CMakeLists.txt" ]; then
        echo -e "${RED}错误：${sub_path} 源码准备失败${NC}"
        echo -e "缺少文件：${sub_dir}/CMakeLists.txt"
        exit 1
    fi

    echo -e "${GREEN}${sub_path} 源码准备完成${NC}"
}

if [ "${#SUBMODULE_PATHS[@]}" -eq 0 ]; then
    echo -e "${RED}警告：未在 .gitmodules 中发现任何子模块配置${NC}"
else
    for sub_path in "${SUBMODULE_PATHS[@]}"; do
        prepare_submodule "${sub_path}"
    done
fi


echo -e "${GREEN}[8/8] 正在验证开发环境...${NC}"


check_tool() {
    local name="$1"
    shift

    printf "  %-20s" "${name}"

    if "$@" >/dev/null 2>&1; then
        echo -e "${GREEN}已安装${NC}"
    else
        echo -e "${RED}未找到${NC}"
    fi
}

check_tool "CMake" cmake --version
check_tool "Ninja" ninja --version
check_tool "G++" g++ --version
check_tool "Clang" clang --version
check_tool "Clangd" clangd --version
check_tool "Clang-Format" clang-format --version
check_tool "GDB" gdb --version
check_tool "Git" git --version
check_tool "qmake5" qmake5 --version
check_tool "qmake6" qmake6 --version

echo
check_submodule() {
    local sub_path="$1"

    printf "  %-20s" "${sub_path}"

    if [ -f "${PROJECT_ROOT}/${sub_path}/CMakeLists.txt" ]; then
        echo -e "${GREEN}已就绪${NC}"
    else
        echo -e "${RED}缺失${NC}"
    fi
}

echo -e "${YELLOW}第三方依赖源码：${NC}"
for sub_path in "${SUBMODULE_PATHS[@]}"; do
    check_submodule "${sub_path}"
done

echo
echo -e "${BLUE}========================================================${NC}"
echo -e "${GREEN}开发环境安装完成${NC}"
echo -e "${BLUE}========================================================${NC}"
echo
echo -e "${YELLOW}首次配置 Qt6：${NC}"
echo -e "  cmake -S . -B build-qt6 -G Ninja \\"
echo -e "    -DCMAKE_BUILD_TYPE=Debug \\"
echo -e "    -DQT_VERSION_MAJOR=6"
echo
echo -e "${YELLOW}首次配置 Qt5：${NC}"
echo -e "  cmake -S . -B build-qt5 -G Ninja \\"
echo -e "    -DCMAKE_BUILD_TYPE=Debug \\"
echo -e "    -DQT_VERSION_MAJOR=5"
echo
