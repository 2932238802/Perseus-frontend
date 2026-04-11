# 开发环境搭建

> 本文档面向初次参与 Perseus 项目的开发者，从 0 到 1 搭建完整的开发环境。

---

## 1. 克隆项目

```bash
git clone https://github.com/2932238802/Perseus.git
cd Perseus
```

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
| 2 | `build-essential` `cmake` `ninja-build` `clang-18` `gdb` `git` | C++ 编译工具链 |
| 3 | `qt6-base-dev` `qt6-base-dev-tools` + Mesa/XCB 库 | Qt 6 基础框架 + 图形依赖 |
| 4 | `qt6-webengine-dev` `qt6-webchannel-dev` | Qt WebEngine (终端 xterm.js 依赖) |
| 5 | `clangd` `clang-format` | LSP 智能补全 + 代码格式化 |
| 6 | `dbus-x11` `x11-apps` | WSL2 GUI 支持 |

---

## 4. 构建与运行

### 脚本一览

| 脚本 | 命令 | 说明 |
|------|------|------|
| `re-build.sh` | `./re-build.sh` | **完整重建**（推荐首次使用）— 清除 `_build/`，CMake 配置 + 编译 + 运行 |
| `build.sh` | `./build.sh` | **增量编译** + 运行 — 仅编译变更文件，速度快 |
| `gdb-build.sh` | `./gdb-build.sh` | **Debug 重建** — 同 re-build 但使用 Debug 模式，运行后可用 GDB 调试 |
| `clean.sh` | `./clean.sh` | **清除构建** — 删除整个 `_build/` 目录 |
| `makefile` | `make run` / `make gdb` | **快捷入口** — 直接运行或 GDB 调试已编译的程序 |

### 典型工作流

```bash
# 首次构建（从零开始）
./re-build.sh

# 日常开发（修改代码后增量编译）
./build.sh

# 只运行，不编译
make run

# 需要调试时
./gdb-build.sh    # 重建 Debug 版本
make gdb           # GDB 附加调试
```

### 各脚本详解

#### `re-build.sh` — 完整重建

```bash
sudo rm -rf _build                      # 清除旧构建
mkdir _build && cd _build
cmake .. -G Ninja \                     # 使用 Ninja 生成器
    -DCMAKE_C_COMPILER=/usr/bin/clang-18 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-18
cmake --build . --config Release        # Release 编译
./Perseus                               # 运行
```

- 使用 **Clang-18** 作为编译器（非 GCC）
- 使用 **Ninja** 构建（比 Make 更快的并行编译）
- 输出 Release 优化版本

#### `build.sh` — 增量编译

```bash
cd _build
cmake --build . --config Release        # 仅编译变更
./Perseus                               # 运行
```

- 前提：`_build/` 已存在（需要先跑过一次 `re-build.sh`）
- 只重新编译修改过的文件，速度很快

#### `gdb-build.sh` — Debug 构建

与 `re-build.sh` 相同，但去掉了 `--config Release`，使用默认的 Debug 模式编译，保留调试符号。

#### `clean.sh` — 清除构建

```bash
sudo rm -rf _build
```

#### `makefile` — 快捷命令

```makefile
run:    # make run → 直接运行
gdb:    # make gdb → GDB 调试
```

---
