# UI 组件

> 本文档详细介绍 Perseus 的 UI 组件库。

---

## 1. 组件总览

Perseus 采用 Qt Widgets 构建，所有 UI 组件位于 `src/view/` 目录下：

```
src/view/
├── LosEditorUi/          # 代码编辑器
├── LosEditorTabUi/       # 编辑器标签页
├── LosTerminalUi/       # 终端面板
├── LosTerminalTabsUi/   # 终端标签页
├── LosFileTreeUi/      # 文件树
├── LosCommandUi/        # 命令面板
├── LosCommandArgsUi/    # 命令参数输入
├── LosSettingsUi/       # 设置面板
├── LosPlugUi/          # 插件面板
├── LosPluginDetailUi/   # 插件详情
├── LosIssuesUi/         # 问题面板
├── LosFloatingPanelUi/  # 浮动面板
├── LosCompleterUi/     # 自动补全
└── LosLineNumberUi/    # 行号显示
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

## 3. 组件关系图

```
MainWindow (Perseus)
│
├── ┌──────────────────────────┐
│  │    中央区域 (QStackedWidget)   │
│  │    ┌──────────────────┐   │
│  │    │LosEditorTabUi  │   │
│  │    │  (编辑器)     │   │
│  │    └──────────────────┘   │
│  │    ┌──────────────────┐   │
│  │    │LosTerminalTabsUi│   │
│  │    │  (终端)      │   │
│  │    └──────────────────┘   │
│  └──────────────────────────┘
│
├── 左侧边栏 (QTreeWidget)
│    └── LosFileTreeUi (文件树)
│
├── 右侧边栏 (可选)
│    ├── LosSettingsUi
│    ├── LosPlugUi
│    └── LosIssuesUi
│
├── 浮动面板
│    ├── LosCommandUi
│    ├── LosCommandArgsUi
│    └── LosGotoLinePopupUi
```