# 架构总览

> 本文档介绍 Perseus 项目的整体架构、技术栈和模块关系。

---

## 1. 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | C++17 (标准) |
| 框架 | Qt 6.x (Widgets, Network, Concurrent, WebEngineWidgets, WebChannel) |
| 构建 | CMake ≥ 3.18, Ninja |
| 平台 | Ubuntu / WSL2 |
| LSP | clangd (C++), rust-analyzer (Rust), neocmakelsp (CMake), pyright (Python) |
| 终端 | xterm.js + QWebChannel + QWebEngineView |

---

## 2. 架构分层

Perseus 采用经典的三层架构设计：

```
┌─────────────────────────────────────────────┐
│                 View 层                     │
│   (UI 组件: 编辑器、终端、文件树、命令面板)    │
├─────────────────────────────────────────────┤
│                 Model 层                     │
│   (数据模型: 文件树、文件节点、文件路径)      │
├─────────────────────────────────────────────┤
│                 Core 层                     │
│   (核心逻辑: LSP、运行器、配置、工具链)      │
├─────────────────────────────────────────────┤
│               Common 层                     │
│   (公共工具: 常量、工具函数、日志)        │
└─────────────────────────────────────────────┘
```

### 各层职责

| 层级 | 职责 |
|------|------|
| View | 负责 UI 渲染和用户交互，将用户操作转发给 Core 层处理 |
| Model | 负责数据结构和状态管理，不包含业务逻辑 |
| Core | 负责核心业务逻辑：编译运行、LSP 通信、配置解析 |
| Common | 公共工具：常量定义、辅助函数、日志系统 |

---

## 3. 模块关系图

```
                    ┌──────────────┐
                    │   Perseus   │
                    │ (主窗口)    │
                    └──────┬─────┘
                           │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
    ┌──────────┐   ┌──────────┐   ┌──────────┐
    │ View 层  │   │Model 层  │   │Core 层  │
    └────┬────┘   └────┬────┘   └────┬────┘
         │              │              │
    ┌────┴────┐   ┌───┴───┐   ┌─────┴─────┐
    │EditorUi │   │File   │   │LosRunManager│
    │Terminal │   │Node   │   │LosLspMgr   │
    │FileTree │   │Path  │   │LosConfigMgr│
    │Command │   │Tree │   │LosToolChain│
    │Settings │   │Model│   │LosFormat  │
    └─────────┘   └─────┘   └───────────┘
```

---

## 4. 信号总线架构

Perseus 使用 **LosRouter** 作为全局信号路由器，实现模块间的松耦合通信：

```
┌─────────────────────────────��────────────┐
│               LosRouter                   │
│            (信号总线单例)                 │
└──────────────────┬─────────────────────┘
                    │
    ┌───────────────┼───────────────┐
    │               │               │
    ▼               ▼               ▼
  信号 A         信号 B          信号 C
(View → Core)   (Core → View)   (Core → Core)
```

### 信号命名规范

| 前缀 | 方向 | 示例 |
|------|------|------|
| `_cmd_` | Controller → Action | `_cmd_gotoFile` |
| `_cmd_lsp_request_` | View → LSP | `_cmd_lsp_request_openFile` |
| `_cmd_lsp_result_` | LSP → View | `_cmd_lsp_result_completion` |
| `_cmd_fileDirty` | View → 广播 | `_cmd_fileDirty` |

---

## 5. 核心流程

### 5.1 打开文件流程

```
用户双击文件树
       │
       ▼
LosFileTreeUi --_cmd_gotoFile--> LosEditorTabUi
                                         │
                                         ▼
                                 LosLspManager.openFile()
                                         │
                                         ▼
                                 LSP Server (clangd)
                                         │
                                         ▼
                                 _cmd_lsp_result_completion --> LosCompleterUi
```

### 5.2 构建运行流程

```
用户点击构建按钮
       │
       ▼
Perseus.onRunSingleFileBtnClicked()
       │
       ▼
LosToolChainManager 检查工具链
       │
       ▼
LosRunManager.execute()
       │
       ▼
LosAbstractRunner (具体 Runner)
       │
       ▼
QProcess 执行编译
       │
       ▼
输出到 LosTerminalUi
```

---

## 6. 目录结构

```
src/
├── common/               # 公共工具
│   ├── constants/       # 常量定义
│   ├── enum/           # 枚举定义
│   └── util/           # 工具函数
├── core/                # 核心业务逻辑
│   ├── LosConfig/      # 配置解析
│   ├── LosFormat/     # 代码格式化
│   ├── LosLsp/       # LSP 客户端
│   ├── LosNet/       # 网络通信
│   ├── LosRunner/    # 编译运行器
│   ├── LosToolChain/ # 工具链管理
│   └── ...
├── models/              # 数据模型
│   ├── LosFileNode/    # 文件节点
│   ├── LosFilePath/   # 文件路径
│   └── LosFileTreeModel/ # 文件树模型
└── view/               # UI 组件
    ├── LosEditorUi/   # 代码编辑器
    ├── LosTerminalUi/ # 终端面板
    ├── LosFileTreeUi/ # 文件树
    └── ...
```

---

## 7. 依赖方向

```
View ──▶ Model (使用数据)
View ──▶ Core (调用服务)
Core ──▶ Model (操作数据)
Core ──▶ Core (互相调用)
Common ──▶ 所有层 (提供工具)
```

**重要原则**：
- View 层不能直接调用其他 View 层的内部方法
- Model 层不包含任何业务逻辑
- Core 层通过信号与 View 通信，保持松耦合