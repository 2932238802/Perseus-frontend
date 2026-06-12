# 架构总览

> 本文档介绍 Perseus 项目的整体架构、技术栈和模块关系。

---

## 1. 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | **C++23**（用到 `std::stacktrace`，链接 `stdc++exp`） |
| 框架 | Qt 6.x（Widgets, Network, Concurrent, WebEngineWidgets, WebChannel, Test, Core5Compat） |
| 构建 | CMake ≥ 3.18, Ninja |
| 编译器 | Clang-18（推荐）/ GCC |
| 平台 | Ubuntu 24.04 / WSL2 |
| LSP | clangd (C++), rust-analyzer (Rust), neocmakelsp (CMake), pyright (Python) |
| 终端 | xterm.js + QWebChannel + QWebEngineView |
| Git | libgit2 v1.9.4（third_party submodule，静态链接） |

> 📌 `CMakeLists.txt` 中 `set(CMAKE_CXX_STANDARD 23)`。整个项目源码（`src/`）被编译为静态库
> `PerseusCore`，主程序 `Perseus` 与测试目标都链接它。

---

## 2. 架构分层

Perseus 采用经典的分层架构 + 全局信号总线设计：

```
┌──────────────────────────────────────────────────────┐
│                      View 层                          │
│  编辑器 / 终端 / 文件树 / 命令面板 / 设置 / AI 助手 / 登录  │
├──────────────────────────────────────────────────────┤
│                     Model 层                          │
│       文件树模型 / 文件节点 / 文件路径 / 文件上下文          │
├──────────────────────────────────────────────────────┤
│                      Core 层                          │
│  LSP / 运行器 / 配置 / 工具链 / 网络 / AI / Git / 主题      │
├──────────────────────────────────────────────────────┤
│                     Common 层                         │
│              常量 / 工具函数 / 日志                       │
└──────────────────────────────────────────────────────┘
        ▲                                       ▲
        └────────── LosRouter 信号总线 ──────────┘
              (贯穿各层，承载所有跨层通信)
```

### 各层职责

| 层级 | 职责 |
|------|------|
| View | UI 渲染与用户交互；通过 LosRouter 把操作转发给 Core，不直接依赖 Core 服务对象 |
| Model | 数据结构与状态（文件树、节点、路径、上下文），不含业务逻辑 |
| Core | 核心业务：编译运行、LSP 通信、配置解析、网络请求、AI 调用、Git 追踪、主题管理 |
| Common | 公共工具：常量定义、辅助函数、日志系统 |

**分层铁律**：业务逻辑严禁进入 UI 层；所有跨层、跨模块通信一律通过 **LosRouter** 信号完成。

---

## 3. 模块关系图

```
                         ┌──────────────┐
                         │   Perseus    │  (主窗口 QMainWindow)
                         │  装配所有模块 │
                         └──────┬───────┘
                                │ 持有/初始化
        ┌──────────────┬────────┼────────┬──────────────┐
        ▼              ▼        ▼        ▼              ▼
  ┌──────────┐  ┌──────────┐ ┌─────┐ ┌──────────┐ ┌──────────┐
  │ View 组件 │  │ Model    │ │Core │ │ 单例服务  │ │ 单例服务  │
  │ EditorTab │  │ TreeModel│ │ Mgr │ │ LosRouter │ │ LosNet    │
  │ FileTree  │  │ FileNode │ │ Run │ │ LosGitMgr │ │ LosTheme  │
  │ Terminal  │  │ FilePath │ │ Lsp │ │ LosShortcut│ │ LosAgent  │
  │ Command   │  │ Context  │ │ Cfg │ │ LosFileSys │ │ ...       │
  │ Settings  │  └──────────┘ │Tool │ └──────────┘ └──────────┘
  │ AgentUi   │               └─────┘
  │ AuthUi    │                  ▲              ▲
  └────┬──────┘                  │              │
       └─────────── LosRouter 信号总线 ──────────┘
```

---

## 4. 信号总线架构（LosRouter）

Perseus 使用 **`LosCore::LosRouter`** 作为全局信号路由器（单例），实现模块间松耦合通信。
所有信号集中声明在 `src/core/LosRouter/LosRouter.h`，模块只 `connect` / `emit`，互不直接引用。

```
┌────────────────────────────────────────────┐
│              LosRouter::instance()           │
│              (全局信号总线单例)               │
└──────────────────┬───────────────────────────┘
                    │  emit / connect
   ┌────────────┬───┼────────────┬─────────────┐
   ▼            ▼   ▼            ▼             ▼
 LSP 信号    文件信号  运行/工具链  网络/认证/AI   Git/主题/工具栏
```

### 信号分组（节选）

| 分组 | 代表信号 | 方向 |
|------|----------|------|
| LSP 请求 | `_cmd_lsp_request_openFile` / `_completion` / `_hover` / `_semantic` | View → LSP |
| LSP 结果 | `_cmd_lsp_result_completion` / `_diagnostics` / `_definition` / `_hover` | LSP → View |
| 文件操作 | `_cmd_gotoFile` / `_fileChanged` / `_fileDirty` / `_fileRenamed` / `_fileTreeDone` | 广播 |
| 工具链 | `_cmd_toolChainMissing` / `_toolChainReady` / `_buildToolReady` / `_autoInstallTool` | Core ↔ View |
| 运行脚本 | `_cmd_runScript` / `_command_exeScript` | View → Core |
| 工具栏 | `_cmd_chooseFileBtnClick` / `_chooseDirBtnClick` / `_runBtnClick` / `_settingBtnClick` | View → 主窗 |
| 网络/插件 | `_cmd_net_pluginReply` / `_net_downloadFinished` / `_openPluginDetail` | Core → View |
| 认证 | `_cmd_authBtnClick` / `_auth_login_request` / `_auth_response` / `_auth_loginStateChanged` / `_needAuth` | 双向 |
| AI 助手 | `_cmd_agent_sendMessage` / `_agent_reply` / `_agent_addProvider_*` / `_agent_listModels_*` / `_agent_listProviders_*` | 双向 |
| Git | `_cmd_initGitProject` / `_getBranchName` / `_gitStatusUpdated` | 双向 |
| 主题 | `_cmd_themeChanged` | 广播 |

### 信号命名规范

| 前缀 | 含义 | 示例 |
|------|------|------|
| `_cmd_` | 通用命令 / 动作 | `_cmd_gotoFile` |
| `_cmd_lsp_request_` | View → LSP 请求 | `_cmd_lsp_request_openFile` |
| `_cmd_lsp_result_` | LSP → View 结果 | `_cmd_lsp_result_completion` |
| `_cmd_net_` | 网络层结果回传 | `_cmd_net_pluginReply` |
| `_cmd_auth_` | 认证相关 | `_cmd_auth_response` |
| `_cmd_agent_` | AI 助手相关 | `_cmd_agent_reply` |

### 使用方式

```cpp
// 连接（观察者）
connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_gotoFile,
        this, &SomeClass::onGotoFile);

// 触发（发布者）
emit LosCore::LosRouter::instance()._cmd_gotoFile(filePath, line);
```

---

## 5. 核心流程

### 5.1 打开文件流程

```
用户双击文件树
       │
       ▼
LosFileTreeUi --_cmd_gotoFile--> LosEditorTabUi 打开/切换标签
       │
       ▼
LosEditorUi 发出 _cmd_lsp_request_openFile(path, content)
       │
       ▼
LosLspManager → 对应 LSP 客户端 (clangd 等) didOpen
       │
       ▼
LSP 回包 → _cmd_lsp_result_diagnostics / _semanticTokens
       │
       ▼
LosEditorUi 渲染诊断 / 语义高亮；LosIssuesUi 列出诊断
```

### 5.2 构建运行流程

```
用户点击运行按钮 (_cmd_runBtnClick)
       │
       ▼
Perseus 判断单文件 / CMake 项目
       │
       ▼
LosToolChainManager 检查工具链
       │   └── 缺失 → _cmd_toolChainMissing → LosToolMissUi 引导下载
       ▼
LosRunManager.execute() → 选择具体 Runner (工厂)
       │
       ▼
QProcess 编译/运行 → 输出经 _cmd 日志信号 → LosTerminalUi / 输出面板
```

### 5.3 AI 助手对话流程

```
用户在 LosAgentUi 输入消息并选择 provider / model
       │
       ▼
_cmd_agent_sendMessage(msg, provider, model)
       │
       ▼
LosAgentManager.handleMessage() → LosNet.requestAgentChat()
       │
       ▼
后端返回 → LosNet.dealAgentChatReply() → _cmd_agent_reply(suc, reply)
       │
       ▼
LosAgentUi 渲染回复（支持 Markdown 预览）
```

### 5.4 Git 状态追踪流程

```
打开项目目录 → _cmd_initGitProject(path)
       │
       ▼
LosGitManager.open() (libgit2 git_repository_open)
       │
       ▼
status() 计算各文件状态 → _cmd_gitStatusUpdated / _getBranchName
       │
       ▼
LosFileTreeDelegate 根据文件状态着色；状态栏显示分支名
```

---

## 6. 依赖方向

```
View   ──▶ Model        (读取数据模型)
View   ──▶ LosRouter    (发信号，不直接调 Core 服务)
Core   ──▶ Model        (操作数据)
Core   ──▶ LosRouter    (发/收信号)
Core   ──▶ Core         (单例间可互相调用)
Common ──▶ 所有层        (提供常量/工具/日志)
```

**重要原则**：
- View 层不能直接调用其他 View 内部方法，跨组件通信走 LosRouter。
- Model 层不包含业务逻辑。
- Core 层服务（多为单例）通过 LosRouter 信号与 View 解耦。

---

## 7. 单例服务一览

| 单例 | 职责 | 获取方式 |
|------|------|----------|
| `LosRouter` | 全局信号总线 | `LosRouter::instance()` |
| `LosNet` | 网络请求（插件市场 / 认证 / AI） | `LosNet::instance()` |
| `LosGitManager` | Git 仓库追踪（libgit2） | `LosGitManager::instance()` |
| `LosAgentManager` | AI 助手消息处理 | `LosAgentManager::instance()` |
| `LosThemeManager` | 主题切换与 QSS 生成 | `LosThemeManager::instance()` |
| `LosShortcutManager` | 快捷键注册 | `LosShortcutManager::instance()` |
| `LosFileSystem` | 文件系统操作 | `LosFileSystem::instance()` |

> 全部采用 **Meyers Singleton**（函数内局部 `static` 实例 + 删除拷贝构造/赋值），
> 线程安全、无需手动释放。详见 [`design-patterns.md`](./design-patterns.md)。
