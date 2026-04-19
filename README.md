# 🚀 Perseus IDE

![C++17](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.x-brightgreen.svg)
![Platform](https://img.shields.io/badge/Platform-Ubuntu%20%7C%20WSL2-lightgrey.svg)

**Perseus** 是一款基于 C++17 和 Qt 6 构建的轻量级桌面 IDE，参考 VSCode 设计，专为现代 C++ 开发者打造。

---

## ✨ 核心特性

### 代码编辑与智能补全
- 基于 LSP (Language Server Protocol) 的代码智能补全
- 支持多种语言：**C++** (clangd), **Rust** (rust-analyzer), **CMake** (neocmakelsp), **Python** (pyright)
- 语法高亮：正则模式 + LSP 语义标记
- 代码格式化：ClangFormat (C++), Neocmakelsp (CMake)
- 跳转到定义、查看 Hover 信息
- 行号显示、当前行高亮、代码折叠标记

### 项目构建与运行
- **单文件编译运行**：C++、Python、Rust 直接运行单文件
- **CMake 项目支持**：完整的 CMake 项目编译、构建、运行
- 集成终端面板，实时显示编译输出
- 构建错误诊断，点击可直接跳转到对应文件和行

### 终端集成
- 嵌入式 Web 终端 (xterm.js + Qt WebEngine)
- 多终端标签支持
- 跨平台：Linux (script 命令)、Windows (PowerShell)

### 文件浏览
- 文件树视图，支持目录展开/折叠
- 右键菜单：新建、复制、粘贴、删除、重命名
- 展开状态持久化
- 外部路径打开 (Explorer/Finder)

### 命令面板
- `Ctrl+Shift+P` 打开命令面板
- 支持所有内置命令
- 支持插件注册自定义命令
- 支持脚本命令执行

### 插件系统
- 插件市场浏览
- 本地插件加载
- 插件详情展示
- 命令、快捷键扩展

### 配置与设置
- 工具链自动检测 (clang, g++, python, rustc 等)
- 工具链配置管理 (JSON 格式)
- LSP 工具配置
- 设置面板 UI

### 其他特性
- 多标签编辑器
- 命令参数输入对话框
- 跳转到行 (`Ctrl+G`)
- 问题面板 (Lint 诊断结果)
- 会话历史恢复 (上次打开的文件、目录)
- 快捷键支持

---

## 🛠️ 快速上手

### 环境要求

| 类别 | 要求 |
|------|------|
| 操作系统 | Ubuntu 24.04 / WSL2 (推荐) |
| 编译器 | Clang-18 / GCC |
| Qt | Qt 6.x |
| 构建工具 | CMake ≥ 3.18, Ninja |

### 安装依赖

```bash
# 克隆项目
git clone https://github.com/2932238802/Perseus.git
cd Perseus

# 一键安装依赖
chmod +x env.sh
./env.sh
```

### 构建与运行

```bash
# 完整重建（首次）
./re-build.sh

# 增量编译（日常开发）
./build.sh
```

### 快捷键一览

| 快捷键 | 功能 |
|--------|------|
| `Ctrl+O` | 打开文件 |
| `Ctrl+S` | 保存文件 |
| `Ctrl+G` | 跳转到行 |
| `Ctrl+Shift+P` | 命令面板 |
| `Ctrl+B` | 构建项目 |
| `Ctrl+Shift+B` | 运行项目 |
| `Ctrl+` | 放大界面 |
| `Ctrl-` | 缩小界面 |

---

## 📖 文档导航

| 目录 | 内容 |
|------|------|
| [`docs/1_architecture/`](docs/1_architecture/) | 架构设计 — 模块划分、设计模式 |
| [`docs/2_extensions/`](docs/2_extensions/) | 插件系统 — 协议、开发指南 |
| [`docs/3_development/`](docs/3_development/) | 开发指南 — 构建流程、环境配置 |
| [`docs/4_ui_ux/`](docs/4_ui_ux/) | 界面与交互 — 组件、主题、快捷键 |

---

## 🤝 参与协作

### 分支管理

```
main                 ← 稳定主线
  └── dev-<功能名>      ← 功能开发分支
```

### 提交规范

```
[add]  新增功能
[fix]  修复 Bug
[fit]  完善优化
[modify] 重构
[del]  删除代码
[docs] 文档变更
```

### 开发流程

1. 创建功能分支：`git checkout -b dev-功能名`
2. 开发并提交
3. Rebase 到最新 main：`git rebase origin/main`
4. 推送并发起 PR

---

### 💡 提示：更多开发细节请参考 [`docs/3_development/setup.md`](docs/3_development/setup.md)