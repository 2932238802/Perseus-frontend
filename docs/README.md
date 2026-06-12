# Perseus IDE — 文档中心

> **Perseus** 是一个基于 **C++23 / Qt 6** 构建的轻量级桌面 IDE，参考 VSCode 设计，
> 面向现代 C++ / Rust / Python / CMake 多语言开发。

---

## 文档导航

| 编号 | 目录 | 内容 |
|:----:|------|------|
| 1 | [`1_architecture/`](./1_architecture/) | 架构设计 — 整体架构、模块划分、信号总线（LosRouter）、设计模式 |
| 2 | [`2_extensions/`](./2_extensions/) | 插件系统 — 插件协议、开发指南、分发机制 |
| 3 | [`3_development/`](./3_development/) | 开发指南 — 构建流程、环境配置、Git 工作流、libgit2 集成 |
| 4 | [`4_ui_ux/`](./4_ui_ux/) | 界面与交互 — UI 组件、主题系统、快捷键、布局设计 |
| — | [`AGENTS.md`](./AGENTS.md) | AI 代理（Cursor / Copilot / CLI）协作说明与编码规范 |

---

## 项目总览

### 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | **C++23**（用到 `std::stacktrace` 等特性，需链接 `stdc++exp`） |
| 框架 | Qt 6（Widgets / Network / Concurrent / WebEngineWidgets / WebChannel / Test / Core5Compat） |
| 构建 | CMake ≥ 3.18 + Ninja |
| 编译器 | Clang-18（推荐）/ GCC |
| 平台 | Ubuntu 24.04 / WSL2 |
| LSP | clangd (C/C++)、rust-analyzer (Rust)、neocmakelsp (CMake)、pyright (Python) |
| 终端 | xterm.js + QWebChannel + QWebEngineView |
| Git | libgit2 v1.9.4（git submodule，静态链接） |

---

## 核心能力速览

| 能力域 | 说明 | 相关模块 |
|--------|------|----------|
| 代码编辑 | 多标签编辑器、语法高亮、代码折叠、行号、当前行高亮 | `LosEditorUi` / `LosEditorTabUi` / `LosHighlighter` |
| 智能补全 | 基于 LSP 的补全、Hover、定义跳转、语义高亮、诊断 | `LosLsp*` / `LosCompleterUi` |
| 构建运行 | 单文件运行（C++/Python/Rust）、CMake 项目构建运行 | `LosRunner` / `LosToolChainManager` |
| 集成终端 | WebEngine 嵌入式终端、多终端标签 | `LosTerminalUi` / `LosTerminalTabsUi` |
| 文件管理 | 文件树、右键菜单、展开态持久化、Git 状态着色 | `LosFileTreeUi` / `LosFileTreeModel` / `LosGitManager` |
| 命令系统 | 命令面板、命令参数输入、插件命令注册 | `LosCommandUi` / `LosCommandArgsUi` |
| 插件系统 | 插件市场浏览、本地插件加载、插件详情 | `LosPlugUi` / `LosPluginDetailUi` / `LosNet` |
| AI 助手 | 内置 AI 对话、多服务商/模型配置、API Key 管理 | `LosAgent` / `LosAgentUi` / `LosAgentKeyUi` |
| 账户认证 | 登录 / 注册 / 自动登录（Token） | `LosAuthUi` / `LosNet` |
| 主题系统 | 多主题切换（默认 dracula）、QSS Token 注入 | `LosThemeManager` |
| 配置工具链 | 工具链自动检测、配置管理、缺失工具引导下载 | `LosToolChainManager` / `LosToolMissUi` |
| 跨模块通信 | 全局信号总线，解耦 View 与 Core | `LosRouter` |

---

## 目录结构总览

```
src/
├── main.cpp                  # 程序入口
├── Perseus.{h,cpp}           # 主窗口（QMainWindow），装配各模块
├── common/                   # 公共层
│   ├── constants/            # 常量（ConstantsClass / ConstantsNum / ConstantsStr）
│   └── util/                 # 工具函数
├── core/                     # 核心业务层
│   ├── LosRouter/            # 全局信号总线（单例）
│   ├── LosLsp/               # LSP 客户端与管理器
│   ├── LosRunner/            # 编译运行器（含脚本运行）
│   ├── LosToolChainManager/  # 工具链检测与配置
│   ├── LosConfig/            # 项目配置解析
│   ├── LosFormat/            # 代码格式化
│   ├── LosHighlighter/       # 语法高亮
│   ├── LosNet/               # 网络层（单例，插件市场/认证/AI）
│   ├── LosAgent/             # AI 助手（Manager + Client）
│   ├── LosGitManager/        # Git 集成（libgit2，单例）
│   ├── LosTheme/             # 主题管理器
│   ├── LosEncodingDetector/  # 文件编码检测
│   ├── LosShortcutManager/   # 快捷键管理（单例）
│   ├── LosSesssion/          # 会话历史
│   ├── LosState/             # 全局状态
│   ├── LosTerminal/          # 终端后端
│   ├── LosPanelManager/      # 面板管理
│   ├── LosPlatform/          # 平台差异封装
│   ├── LosFileSystem/        # 文件系统操作（单例）
│   └── LosLog/               # 日志
├── models/                   # 数据模型层
│   ├── LosFileTreeModel/     # 文件树模型
│   ├── LosFileNode/          # 文件节点
│   ├── LosFilePath/          # 文件路径
│   └── LosFileContext/       # 文件上下文
└── view/                     # UI 视图层（详见 4_ui_ux/components.md）
    ├── Perseus.ui            # 主窗口布局
    ├── style/                # QSS 样式与主题
    └── Los*Ui/               # 各 UI 组件
```

> ⚠️ `third_party/`（libgit2）以 git submodule 形式存在，**clone 后必须执行**
> `git submodule update --init --recursive`，否则 CMake 配置失败。详见
> [`3_development/libgit2-integration.md`](./3_development/libgit2-integration.md)。

---

## 快速开始

```bash
# 1. 克隆（含子模块）
git clone --recurse-submodules <仓库地址>
cd Perseus

# 2. 一键安装依赖
chmod +x env.sh && ./env.sh

# 3. 首次完整构建并运行
./re-build.sh

# 4. 日常增量编译并运行
./rbuild.sh        # Release
./bbuild.sh        # Debug
```

更多细节见 [`3_development/setup.md`](./3_development/setup.md)。
