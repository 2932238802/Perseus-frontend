# UI 组件

> 本文档详细介绍 Perseus 的 UI 组件库。

---

## 1. 组件总览

Perseus 采用 Qt Widgets 构建（静态布局优先用 `.ui` 文件 + `setupUi`），所有 UI 组件位于 `src/view/` 目录下：

```
src/view/
├── Perseus.ui            # 主窗口布局
├── style/                # QSS 样式与多主题（见 theme.md）
│
├── LosEditorUi/          # 代码编辑器
├── LosEditorTabUi/       # 编辑器标签页
├── LosLineNumberUi/      # 行号显示
├── LosCompleterUi/       # 自动补全弹窗
│
├── LosTerminalUi/        # 终端面板（WebEngine）
├── LosTerminalTabsUi/    # 终端标签页
│
├── LosFileTreeUi/        # 文件树
├── LosFileTreeDelegate/  # 文件树绘制委托（Git 状态着色等）
│
├── LosToolBarUi/         # 顶部/活动栏工具栏
├── LosStateBarUi/        # 底部状态栏（分支名、编码等）
├── LosDropdownButtonUi/  # 下拉按钮控件
│
├── LosCommandUi/         # 命令面板（Ctrl+Shift+P）
├── LosCommandArgsUi/     # 命令参数输入
│
├── LosSettingsUi/        # 设置面板
├── LosToolMissUi/        # 工具链缺失引导
│
├── LosPlugUi/            # 插件面板（市场）
├── LosPluginDetailUi/    # 插件详情
│
├── LosIssuesUi/          # 问题面板（诊断）
├── LosFloatingPanelUi/   # 浮动面板容器（含 LosGotoLinePopupUi）
│
├── LosAuthUi/            # 登录 / 注册对话框
├── LosAgentUi/           # AI 助手对话面板
├── LosAgentKeyUi/        # AI 服务商 / API Key 配置
└── LosPreview/           # Markdown 预览
```

---

## 2. 核心组件

### 2.1 LosEditorUi - 代码编辑器

**文件位置**: `src/view/LosEditorUi/`

**继承**: `QPlainTextEdit`

**主要功能**:
- 代码编辑
- 语法高亮
- 代码补全
- 错误诊断显示
- 跳转到指定行
- 代码格式化

**核心方法**:

| 方法 | 功能 |
|------|------|
| `showCompletion()` | 显示自动补全弹窗 |
| `showDiagnostic()` | 显示 LSP 诊断结果 |
| `gotoLine(int line)` | 跳转到指定行 |
| `format()` | 代码格式化 |
| `save()` | 保存文件 |
| `getWordUnderCursor()` | 获取光标处的单词 |
| `isDirty()` | 检查文件是否已修改 |

**信号接口**:

| 信号 | 说明 |
|------|------|
| `_cmd_fileDirty(filePath, isDirty)` | 文件脏标志变化 |
| `_cmd_lsp_request_textChanged()` | 文本变化请求 |
| `_cmd_lsp_request_semantic()` | 语义请求 |
| `_cmd_whereDefine()` | 跳转定义请求 |

---

### 2.2 LosEditorTabUi - 编辑器标签页

**文件位置**: `src/view/LosEditorTabUi/`

**主要功能**:
- 多标签管理
- 标签关闭/切换
- 标签持久化

**核心方法**:

| 方法 | 功能 |
|------|------|
| `closeTab(int index)` | 关闭标签 |
| `closeAllTabs()` | 关闭所有标签 |
| `saveTab(int index)` | 保存标签 |
| `saveAllTabs()` | 保存所有标签 |
| `openFile(QString path)` | 打开文件 |
| `formatTab()` | 格式化当前标签 |
| `getCurEditor()` | 获取当前编辑器 |
| `tabCount()` | 获取标签数量 |
| `getOpenFiles()` | 获取已打开文件列表 |

---

### 2.3 LosTerminalUi / LosTerminalTabsUi - 终端面板

#### LosTerminalUi
**文件位置**: `src/view/LosTerminalUi/`

**主要功能**: 基于 WebEngine 的嵌入式终端

**核心组件**:
- `QWebEngineView` - Web 页面渲染
- `QWebChannel` - JS/Qt 通信

#### LosTerminalTabsUi
**文件位置**: `src/view/LosTerminalTabsUi/`

**主要功能**: 多终端标签管理

**核心方法**:

| 方法 | 功能 |
|------|------|
| `addNewTerminal()` | 新增终端标签 |

---

### 2.4 LosFileTreeUi - 文件树

**文件位置**: `src/view/LosFileTreeUi/`

**主要功能**:
- 文件浏览
- 树形结构显示
- 右键上下文菜单
- 展开状态持久化

**核心方法**:

| 方法 | 功能 |
|------|------|
| `updateExplorer()` | 更新文件树模型 |
| `copyFileOrFolder()` | 复制文件/文件夹 |
| `deleteFileOrFolder()` | 删除文件/文件夹 |
| `expandToFile(QString path)` | 展开到指定文件 |
| `restoreExpandedState()` | 恢复展开状态 |
| `getExpandedPaths()` | 获取展开路径集合 |

---

### 2.5 LosCommandUi - 命令面板

**文件位置**: `src/view/LosCommandUi/`

**主要功能**:
- 命令搜索和执行
- 键盘导航

**核心方法**:

| 方法 | 功能 |
|------|------|
| `showPalette()` | 显示命令面板 |
| `regis(QString id, QString title, QString key, CallBack cb)` | 注册命令 |
| `initScanLocalPlugins()` | 扫描本地插件 |

**快捷键**: `Ctrl+Shift+P`

---

### 2.6 LosCommandArgsUi - 命令参数输入

**文件位置**: `src/view/LosCommandArgsUi/`

**主要功能**: 命令参数输入对话框

**核心方法**:

| 方法 | 功能 |
|------|------|
| `onExeScript(QString script, QString dir, QString args)` | 设置脚本参数 |
| `onRunBtnClicked()` | 执行脚本 |
| `onBrowseBtnClicked()` | 浏览工作目录 |

---

### 2.7 LosSettingsUi - 设置面板

**文件位置**: `src/view/LosSettingsUi/`

**主要功能**: 设置界面管理

**核心方法**:

| 方法 | 功能 |
|------|------|
| `onSaveSettings()` | 保存设置 |
| `onCMakeInstallBtnClicked()` | CMake 安装按钮 |

---

### 2.8 LosPlugUi - 插件面板

**文件位置**: `src/view/LosPlugUi/`

**主要功能**: 插件列表显示

**核心方法**:

| 方法 | 功能 |
|------|------|
| `isEmpty()` | 检查是否为空 |

---

### 2.9 LosPluginDetailUi - 插件详情

**文件位置**: `src/view/LosPluginDetailUi/`

**主要功能**: 插件详情展示

---

### 2.10 LosIssuesUi - 问题面板

**文件位置**: `src/view/LosIssuesUi/`

**主要功能**: LSP 诊断结果显示

**表格列**: Level | Message | Line | File

**核心方法**:

| 方法 | 功能 |
|------|------|
| `onUpdateTable()` | 更新诊断表格 |
| `onTableDoubleClicked()` | 双击跳转 |

---

### 2.11 LosFloatingPanelUi - 浮动面板

**文件位置**: `src/view/LosFloatingPanelUi/`

**主要功能**: 通用浮动弹窗容器

**定位模式**:

| 模式 | 说明 |
|------|------|
| `Center` | 居中显示 |
| `TopRight` | 右上方显示 |
| `TopCenter` | 顶部居中 |

**核心方法**:

| 方法 | 功能 |
|------|------|
| `showAtPosition(Position pos)` | 指定位置显示 |

### 2.12 LosGotoLinePopupUi - 跳转到行弹窗

**文件位置**: `src/view/LosFloatingPanelUi/LosGotoLinePopupUi/`

**主要功能**: 跳转到行号输入

---

### 2.13 LosCompleterUi - 自动补全

**文件位置**: `src/view/LosCompleterUi/`

**主要功能**: 代码补全弹窗

**核心方法**:

| 方法 | 功能 |
|------|------|
| `updateCompletionList(QStringList items)` | 更新补全列表 |

---

### 2.14 LosLineNumberUi - 行号显示

**文件位置**: `src/view/LosLineNumberUi/`

**主要功能**: 行号显示

---

### 2.15 LosAuthUi - 登录 / 注册对话框

**文件位置**: `src/view/LosAuthUi/`

**继承**: `QDialog`

**主要功能**:
- 由活动栏底部认证按钮（`_cmd_authBtnClick`）触发，主窗 `exec()` 弹出
- 内部用 `QStackedWidget` 切换「登录页 / 注册页」
- 表单提交后经 LosRouter 把 `(username, password)` 抛给网络层 `LosNet`，由后端校验
- 网络回包经 `_cmd_auth_response` 回到本窗，在提示标签上显示结果
- 跟随当前主题（`applyTheme`）

**核心方法**:

| 方法 | 功能 |
|------|------|
| `buildLoginPage()` / `buildRegisterPage()` | 构建登录 / 注册页 |
| `onLoginSubmit()` / `onRegisterSubmit()` | 提交表单 |
| `onAuthResult(bool, QString)` | 处理认证回包 |
| `applyTheme(QString)` | 应用主题 |
| `showTip(QString, bool isError)` | 显示提示 |

**相关信号**: `_cmd_auth_login_request` / `_cmd_auth_register_request` / `_cmd_auth_response` / `_cmd_auth_loginStateChanged` / `_cmd_needAuth`

---

### 2.16 LosAgentUi - AI 助手对话面板

**文件位置**: `src/view/LosAgentUi/`

**主要功能**:
- 内置 AI 对话界面，发送消息并展示回复
- 选择服务商（provider）与模型（model）
- 回复内容支持 Markdown 渲染（配合 `LosPreview`）

**核心交互**:
- 发送：`_cmd_agent_sendMessage(msg, provider, model)`
- 接收：`_cmd_agent_reply(suc, reply)`
- 模型/服务商列表：`_cmd_agent_listProviders_*` / `_cmd_agent_listModels_*`

---

### 2.17 LosAgentKeyUi - AI 服务商 / API Key 配置

**文件位置**: `src/view/LosAgentKeyUi/`

**主要功能**:
- 添加 AI 服务商（provider、baseUrl、apiKey、模型列表）
- 拉取并校验可用模型列表

**相关信号**: `_cmd_agent_addProvider_request` / `_cmd_agent_addProvider_response` / `_cmd_agent_listModels_request` / `_cmd_agent_listModels_response`

---

### 2.18 LosPreview - Markdown 预览

**文件位置**: `src/view/LosPreview/`

**主要功能**:
- Markdown 文档预览渲染
- 用于 AI 回复展示、插件 README 预览、标签页预览模式（`_cmd_togglePreview`）

> 样式见 `src/view/style/LosMDPreview_style.h`。

---

### 2.19 LosToolBarUi - 工具栏

**文件位置**: `src/view/LosToolBarUi/`

**主要功能**: 顶部 / 活动栏按钮区，承载打开文件、打开目录、运行、设置、项目模式切换、认证等入口。

**相关信号**: `_cmd_chooseFileBtnClick` / `_cmd_chooseDirBtnClick` / `_cmd_runBtnClick` / `_cmd_settingBtnClick` / `_cmd_projectBtnToggled` / `_cmd_authBtnClick`

---

### 2.20 LosStateBarUi - 状态栏

**文件位置**: `src/view/LosStateBarUi/`

**主要功能**: 底部状态栏，显示 Git 分支名、文件编码、光标位置等信息。

**相关信号**: `_cmd_getBranchName` / `_cmd_gitStatusUpdated`

---

### 2.21 LosDropdownButtonUi - 下拉按钮

**文件位置**: `src/view/LosDropdownButtonUi/`

**主要功能**: 通用下拉按钮控件（如运行配置、主题选择等场景复用）。

---

### 2.22 LosFileTreeDelegate - 文件树委托

**文件位置**: `src/view/LosFileTreeDelegate/`

**继承**: `QStyledItemDelegate`

**主要功能**: 自定义文件树项的绘制——根据 `LosGitManager` 提供的文件状态进行**着色**（新增 / 修改 / 未跟踪等），绘制图标与名称。

---

### 2.23 LosToolMissUi - 工具链缺失引导

**文件位置**: `src/view/LosToolMissUi/`

**主要功能**: 当 `_cmd_toolChainMissing` 触发时弹出，提示缺失的工具并提供下载 / 安装引导。

---

## 3. 组件关系图

```
MainWindow (Perseus)
│
├── 顶部工具栏  LosToolBarUi (打开文件/目录、运行、设置、项目模式、登录)
│
├── 中央区域 (QStackedWidget)
│     ├── LosEditorTabUi    (编辑器，含 LosEditorUi / LosLineNumberUi / LosCompleterUi)
│     └── LosPreview        (Markdown 预览模式)
│
├── 左侧边栏
│     └── LosFileTreeUi     (文件树，用 LosFileTreeDelegate 做 Git 状态着色)
│
├── 右侧 / 侧边面板
│     ├── LosSettingsUi
│     ├── LosPlugUi ──▶ LosPluginDetailUi
│     ├── LosAgentUi ──▶ LosAgentKeyUi   (AI 助手)
│     └── LosIssuesUi
│
├── 底部面板 (LosPanelManager 管理)
│     ├── 输出 / 问题 (LosIssuesUi)
│     └── LosTerminalTabsUi (终端，含 LosTerminalUi)
│
├── 底部状态栏  LosStateBarUi (Git 分支 / 编码 / 光标位置)
│
├── 浮动 / 弹窗
│     ├── LosCommandUi          (命令面板)
│     ├── LosCommandArgsUi      (命令参数)
│     ├── LosGotoLinePopupUi    (跳转到行)
│     ├── LosToolMissUi         (工具链缺失引导)
│     └── LosAuthUi             (登录 / 注册对话框)
│
└── 通用控件  LosDropdownButtonUi / LosFloatingPanelUi
```

> 所有组件间通信通过 **LosRouter** 信号总线完成，UI 层不直接依赖 Core 服务对象。