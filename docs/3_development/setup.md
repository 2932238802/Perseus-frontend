# 开发环境搭建

> 本文档面向初次参与 Perseus 项目的开发者，从 0 到 1 搭建完整的开发环境。

---

## 1. 克隆项目

```bash
# 推荐：clone 时一并拉取 libgit2 子模块
git clone --recurse-submodules https://github.com/2932238802/Perseus.git
cd Perseus

# 若已 clone 但子模块为空：
git submodule update --init --recursive
```

> ⚠️ `third_party/libgit2` 是 git submodule，未拉取时 CMake 配置会失败。
> 详见 [libgit2-integration.md](libgit2-integration.md)。

---

## 2. 开发平台

Perseus 推荐在 **WSL2 (Ubuntu 24.04)** 上开发。

### 为什么选择 WSL2

- Qt 6 WebEngine 在 Linux 下构建最稳定
- 项目脚本全部基于 Bash
- 编译器使用 clang-18，Ubuntu 24.04 原生支持

### WSL2 基本配置

如果你还没有安装 WSL2，在 Windows PowerShell  中执行：

```powershell
wsl --install -d Ubuntu-24.04
```

安装完成后，启动 Ubuntu 并完成初始用户设置。

### GUI 显示

Perseus 是一个桌面 GUI 程序。WSL2 从 Windows 11 开始原生支持 WSLg（自动 GUI 转发），**无需额外配置 X Server**。

验证 GUI 是否可用：

```bash
sudo apt install -y x11-apps
xclock
```

---

## 3. 安装开发环境

项目根目录提供了一键环境配置脚本 `env.sh`，它会安装所有必要的依赖：

```bash
chmod +x env.sh
./env.sh
```

### env.sh 安装内容

| 步骤 | 安装内容 | 用途 |
|:----:|----------|------|
| 1 | `apt-get update` | 更新软件源 |
| 2 | `build-essential` `cmake` `ninja-build` `clang-18` `gdb` `git` | C++ 编译工具链（C++23） |
| 3 | `qt6-base-dev` `qt6-base-dev-tools` `libqt6core5compat6-dev` + Mesa/XCB 库 | Qt 6 基础框架 + Core5Compat + 图形依赖 |
| 4 | `qt6-webengine-dev` `qt6-webchannel-dev` | Qt WebEngine（终端 xterm.js 依赖） |
| 5 | `clangd` `clang-format` | LSP 智能补全 + 代码格式化 |
| 6 | `dbus-x11` `x11-apps` | WSL2 GUI 支持 |

> `libqt6core5compat6-dev` 提供 `Qt6::Core5Compat`，用于 `QTextCodec` 等（文件编码检测 `LosEncodingDetector` 依赖）。
> 另外 libgit2 通过 git submodule 随源码集成，无需 apt 安装。

---

## 4. 构建与运行

### 脚本一览

| 脚本 | 命令 | 说明 |
|------|------|------|
| `re-build.sh` | `./re-build.sh` | **完整重建（Release）**（推荐首次）— 删除 `_build/`，Ninja + Clang-18 配置 + 编译 + 运行 |
| `rbuild.sh` | `./rbuild.sh` | **增量编译并运行（Release）** — 仅编译变更文件，速度快 |
| `bbuild.sh` | `./bbuild.sh` | **增量编译并运行（Debug）** — 默认 Debug 配置增量编译 + 运行 |
| `gdb-build.sh` | `./gdb-build.sh` | **Debug 完整重建** — 同 re-build 但 Debug 模式，保留调试符号，配合 GDB |
| `clean.sh` | `./clean.sh` | **清除构建** — 删除整个 `_build/` 目录 |

### 典型工作流

```bash
# 首次构建（从零开始，Release）
./re-build.sh

# 日常开发（修改代码后增量编译运行）
./rbuild.sh        # Release
./bbuild.sh        # Debug

# 仅运行已编译产物
./_build/Perseus

# 需要 GDB 调试时
./gdb-build.sh
```

### 各脚本详解

#### `re-build.sh` — 完整重建（Release）

```bash
sudo rm -rf _build                      # 清除旧构建
mkdir _build && cd _build
cmake .. -G Ninja \                     # 使用 Ninja 生成器
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang-18 \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++-18
cmake --build . --config Release        # Release 编译
./Perseus                               # 运行
```

- 使用 **Clang-18** 作为编译器（C++23）
- 使用 **Ninja** 构建（比 Make 更快的并行编译）
- 输出 Release 优化版本

#### `rbuild.sh` — 增量编译（Release）

```bash
cd _build
cmake --build . --config Release        # 仅编译变更
./Perseus                               # 运行
```

- 前提：`_build/` 已存在（需先跑过一次 `re-build.sh`）
- 只重新编译修改过的文件，速度很快

#### `bbuild.sh` — 增量编译（Debug）

```bash
cd _build
cmake --build .                         # 默认 Debug 增量编译
./Perseus
```

#### `gdb-build.sh` — Debug 完整重建

与 `re-build.sh` 流程相同，但使用默认 Debug 模式编译，保留调试符号，便于 GDB 调试。

#### `clean.sh` — 清除构建

```bash
sudo rm -rf _build
```

> 💡 **新增 `.cpp/.h` 文件后**：本项目用 `file(GLOB_RECURSE)` 收集源码，
> 新增源文件后必须**重新 CMake 配置**（直接跑 `./re-build.sh`），否则新文件不会被编译。

---
