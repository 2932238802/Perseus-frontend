# 模块详解

> 本文档详细介绍 Perseus 各层核心模块的职责、API 和依赖关系。
> 单例统一采用 **Meyers Singleton**（`static T& instance()`），调用形如 `LosXxx::instance().method()`。

---

## 1. Core 层模块

### 1.1 LosRouter - 全局信号总线（单例）

**文件位置**: `src/core/LosRouter/`

**主要功能**: 全局信号路由器，集中声明所有跨模块信号；各模块只 `connect` / `emit`，互不直接引用，实现解耦。

**信号分组**（详见 [`overview.md` §4](./overview.md)）:

| 分组 | 代表信号 |
|------|----------|
| LSP 请求/结果 | `_cmd_lsp_request_*` / `_cmd_lsp_result_*` |
| 文件操作 | `_cmd_gotoFile` / `_cmd_fileChanged` / `_cmd_fileDirty` / `_cmd_fileRenamed` / `_cmd_fileTreeDone` |
| 工具链 | `_cmd_toolChainMissing` / `_cmd_toolChainReady` / `_cmd_buildToolReady` / `_cmd_autoInstallTool` |
| 运行/脚本 | `_cmd_runScript` / `_cmd_command_exeScript` |
| 工具栏 | `_cmd_chooseFileBtnClick` / `_cmd_chooseDirBtnClick` / `_cmd_runBtnClick` / `_cmd_settingBtnClick` / `_cmd_projectBtnToggled` |
| 网络/插件 | `_cmd_net_pluginReply` / `_cmd_net_downloadFinished` / `_cmd_openPluginDetail` |
| 认证 | `_cmd_authBtnClick` / `_cmd_auth_login_request` / `_cmd_auth_response` / `_cmd_auth_loginStateChanged` / `_cmd_needAuth` |
| AI 助手 | `_cmd_agent_sendMessage` / `_cmd_agent_reply` / `_cmd_agent_addProvider_*` / `_cmd_agent_listModels_*` / `_cmd_agent_listProviders_*` |
| Git | `_cmd_initGitProject` / `_cmd_getBranchName` / `_cmd_gitStatusUpdated` |
| 主题 | `_cmd_themeChanged` |

```cpp
class LosRouter : public QObject
{
    Q_OBJECT
public:
    static LosRouter &instance();
signals:
    // 仅声明信号，无业务实现
    void _cmd_gotoFile(const QString &file_path, int line);
    void _cmd_agent_reply(bool suc, const QString &reply);
    void _cmd_themeChanged(const QString &themeName);
    // ... 详见 LosRouter.h
};
```

---

### 1.2 LosRunManager - 运行管理器

**文件位置**: `src/core/LosRunner/LosRunManager/`

**主要功能**: 根据文件 / 项目类型自动选择对应 Runner，编译并运行。

**支持的语言**:

| 语言 | Runner 类 |
|------|-----------|
| C++（单文件） | `LosSingleCppRunner` |
| CMake 项目 | `LosCmakeRunner` |
| Python | `LosPythonRunner` |
| Rust | `LosRustcRunner` |

**核心 API**:

```cpp
class LosRunManager : public QObject
{
    void execute(const QString &filePath, bool isProject); // 执行文件/项目
    void stop();                                           // 停止所有运行
    void onToolChainReady(LosCommon::LosLanguage lang,
                          LosCommon::LosTool tool,
                          const QString &exePath);          // 工具链就绪回调
};
```

**Runner 抽象基类**:

```cpp
class LosAbstractRunner
{
    virtual void initConnect() = 0;
    virtual void compile()     = 0;
    virtual void run()         = 0;
    virtual void stop()        = 0;
};
```

> 此外 `LosScriptRunner` 负责执行命令面板 / 插件触发的脚本（配合 `_cmd_runScript`、`_cmd_command_exeScript`）。

---

### 1.3 LosLspManager - LSP 管理器

**文件位置**: `src/core/LosLsp/LosLspManager/`

**主要功能**: 管理多语言 LSP 客户端，提供补全、定义跳转、Hover、语义高亮、诊断。

**支持的语言**:

| 语言 | LSP 客户端 | 语言服务器 |
|------|-----------|-----------|
| C++ | `LosLspClangd` | clangd |
| CMake | `LosLspCMake` | neocmakelsp |
| Python | `LosLspPython` | pyright |
| Rust | `LosLspRust` | rust-analyzer |

**核心 API**:

```cpp
class LosLspManager : public QObject
{
    void openFile(const QString &filePath, LosFileContext *ctx);   // didOpen
    void changeFile(const QString &filePath, LosFileContext *ctx); // didChange
    void requestCompletion(const QString &filePath, int line, int col);
    void requestDefinition(const QString &filePath, int line, int col);
    void requestHover(const QString &filePath, int line, int col);
    void requestSemanticTokens(const QString &filePath);
    LosLspClient* getClient(const QString &filePath);
};
```

**LSP 客户端基类**:

```cpp
class LosLspClient : public QObject
{
    virtual void initialize() = 0;
    virtual void shutdown()   = 0;
    virtual void didOpen(const QString &filePath, const QString &content) = 0;
    virtual void didChange(const QString &filePath, const QString &content) = 0;
    virtual void completion(int line, int col) = 0;
    virtual void definition(int line, int col) = 0;
};
```

---

### 1.4 LosConfigManager - 配置管理器

**文件位置**: `src/core/LosConfig/LosConfigManager/`

**主要功能**: 解析项目目录，识别项目类型。

```cpp
class LosConfigManager : public QObject
{
    LosConfig* analyse(const QString &absolutePath); // 分析目录
    LosConfig* create(const QString &filePath);      // 创建配置对象
    bool isInFiles(const QString &fileName);         // 是否核心文件
};
```

**配置实现**:

| 类型 | 类 | 识别文件 |
|------|-----|----------|
| CMake | `LosConfigCMake` | CMakeLists.txt |
| C++ | `LosConfigCpp` | *.cpp, *.h |
| Python | `LosConfigPython` | *.py |
| Rust | `LosConfigRust` | Cargo.toml |

由 `LosConfigFactory::create(LosFileType)` 工厂创建。

---

### 1.5 LosToolChainManager - 工具链管理器

**文件位置**: `src/core/LosToolChainManager/`

**主要功能**: 读取工具链配置（`toolchain_config.json`），检测系统是否安装对应工具，缺失时引导下载。

```cpp
class LosToolChainManager : public QObject
{
    void initConfig();
    void onCheckLanguageToolchain(LosCommon::LosLanguage lang, LosCommon::LosTool tool);
    void onCheckSingleTool(const QString &tool);
};
```

**信号接口**:

| 信号 | 说明 |
|------|------|
| `_cmd_toolChainReady(lang, tool, exePath)` | 语言编译工具就绪 |
| `_cmd_buildToolReady(tool, exePath, args)` | 构建工具就绪 |
| `_cmd_lspReady(tool, exePath, args)` | LSP 工具就绪 |
| `_cmd_toolChainMissing(config)` | 工具缺失（→ LosToolMissUi） |
| `_cmd_autoInstallTool(config)` | 后台自动安装 |

---

### 1.6 LosHighlighter - 代码高亮

**文件位置**: `src/core/LosHighlighter/`

**主要功能**: Qt 语法高亮器，支持正则模式与 LSP 语义模式两套。

```cpp
class LosHighlighter : public QSyntaxHighlighter
{
    void highlightBlock(const QString &text) override;       // Qt 高亮回调
    void updateSemanticTokens(const QJsonObject &data);      // 更新语义标记
    void initSemanticLegend(const QJsonObject &types,
                            const QJsonObject &modifiers);    // 语义图例
};
```

> 具体颜色随当前主题变化，由 `LosThemeManager` 提供 token。

---

### 1.7 LosFormat - 格式化模块

**文件位置**: `src/core/LosFormat/`

| 语言 | 工具 | 实现类 |
|------|------|--------|
| C++ | clang-format | `LosFClangFormat` |
| CMake | neocmakelsp | `LosFNeocmakelsp` |

```cpp
class LosFormatManager
{
    static LosFormatManager& instance();
    QString format(const QString &filePath, const QString &rawContent);
};
```

---

### 1.8 LosNet - 网络层（单例）

**文件位置**: `src/core/LosNet/`

**主要功能**: 统一封装 HTTP 请求，服务于**插件市场**、**账户认证**、**AI 助手**三大场景。
基于 `QNetworkAccessManager`，每个请求配 `requestGet` / `requestPost` + 回调，回包后经 LosRouter 信号回传 UI。

```cpp
class LosNet : public QObject
{
    static LosNet& instance();

    // 插件市场
    void requestPlugin();
    void downloadPlugin(const QString &url, const QString &savePath);
    void fetchPluginReadme(const QString &url, std::function<void(const QString&)> cb);

    // 账户认证
    void requestRegister(const QString &username, const QString &password);
    void requestLogin(const QString &username, const QString &password);
    void requestAutoLogin(const QString &token);

    // AI 助手
    void requestAgentChat(const QString &msg, const QString &provider, const QString &model);
    void requestAgentFetchModels(const QString &baseUrl, const QString &apiKey);
    void requestAgentAddProvider(const QString &provider, const QString &baseUrl,
                                 const QString &apiKey, const QStringList &models);
    void requestListProviders();

private:
    QNetworkAccessManager *L_net;
    void requestGet (const QString &api, std::function<void(const QByteArray&)> cb);
    void requestPost(const QString &api, const QByteArray &body,
                     std::function<void(const QByteArray&)> cb);
};
```

> 每个 `request*` 都有对应的 `deal*Reply` 解析函数，解析后 `emit` LosRouter 信号通知 UI。
> 网络请求**必须连接错误信号**，不假定一定成功。

---

### 1.9 LosAgent - AI 助手

**文件位置**: `src/core/LosAgent/`（`LosAgentManager/` + `LosAgentClient/`）

**主要功能**: 处理 AI 对话请求，转交 `LosNet` 调用后端，回包后经 LosRouter 回传 `LosAgentUi`。

```cpp
class LosAgentManager : public QObject   // 单例
{
    static LosAgentManager& instance();
private:
    void initConnect();
    void handleMessage(const QString &message,
                       const QString &provider, const QString &model);
};
```

**相关信号**: `_cmd_agent_sendMessage`、`_cmd_agent_reply`、`_cmd_agent_addProvider_*`、
`_cmd_agent_listModels_*`、`_cmd_agent_listProviders_*`。UI 侧见
[`4_ui_ux/components.md`](../4_ui_ux/components.md) 的 `LosAgentUi` / `LosAgentKeyUi`。

---

### 1.10 LosGitManager - Git 集成（单例）

**文件位置**: `src/core/LosGitManager/`

**主要功能**: 基于 **libgit2 v1.9.4** 追踪本地仓库状态，为文件树提供 Git 状态着色、分支名显示。
集成方式详见 [`3_development/libgit2-integration.md`](../3_development/libgit2-integration.md)。

```cpp
class LosGitManager : public QObject
{
    static LosGitManager& instance();

    bool open(const QString &path);                       // 打开 .git 仓库
    QString curBranch() const;                            // 当前分支名
    void status();                                        // 刷新所有文件状态
    unsigned int statusOfFile(const QString &absPath);    // 单文件 Git 状态
    bool folderHasBeenChanged(const QString &absFolder);  // 目录是否含改动
    QString lastErr() const;

private slots:
    void onInitGitProject(const QString &path);           // 响应 _cmd_initGitProject

private:
    git_repository *L_repo = nullptr;
    QMap<QString, unsigned int> L_filesStatus;            // 绝对路径 -> 状态
    QSet<QString> L_dirtyFolders;
};
```

> 构造时 `git_libgit2_init()`，析构时 `git_libgit2_shutdown()`。
> 相关信号: `_cmd_initGitProject` / `_cmd_getBranchName` / `_cmd_gitStatusUpdated`。

---

### 1.11 LosThemeManager - 主题管理器（单例）

**文件位置**: `src/core/LosTheme/`

**主要功能**: 管理多主题（默认 `dracula`），按主题 token 渲染 QSS 模板并应用，切换时广播 `_cmd_themeChanged`。

```cpp
class LosThemeManager : public QObject
{
    static LosThemeManager& instance();

    QStringList availableThemes() const;                  // 可用主题列表
    QString currentTheme() const;
    QString currentDisplayName() const;
    bool setTheme(const QString &themeName, bool persist = true); // 切换主题
    QJsonObject themeJson(const QString &themeName) const;

    QString buildMainQss(const QString &themeName) const;             // 主 QSS
    QString buildExtraQss(const QString &tmpl, const QString &theme) const;
    QHash<QString, QString> uiTokens(const QString &themeName) const; // token 表

private:
    QString L_curTheme = QStringLiteral("dracula");
};
```

---

### 1.12 LosEncodingDetector - 编码检测

**文件位置**: `src/core/LosEncodingDetector/`

**主要功能**: 检测文件编码（UTF-8 / GBK 等），配合 `Core5Compat` 的 `QTextCodec` 正确解码非 UTF-8 文件，避免乱码。

---

### 1.13 LosTerminal - 终端后端

**文件位置**: `src/core/LosTerminal/`

**主要功能**: 嵌入式终端后端，基于 `QProcess`。

| 平台 | 命令 |
|------|------|
| Linux | `script -q /dev/null` |
| Windows | `powershell.exe` |

```cpp
class LosTerminal : public QObject
{
    void onTerminalReady();
    void write(const QString &content);
    void onReadyReadStdOut();
    void onReadyReadStdErr();
};
```

---

### 1.14 LosFileSystem - 文件系统（单例）

**文件位置**: `src/core/LosFileSystem/`

```cpp
class LosFileSystem
{
    static LosFileSystem& instance();
    bool createDir(const QString &path);
    bool createFile(const QString &filePath, const QByteArray &data);
    bool removeFile(const QString &filePath);
    bool removeDir(const QString &dirPath);
    bool renameFile(const QString &src, const QString &dst);
};
```

---

### 1.15 LosShortcutManager - 快捷键管理（单例）

**文件位置**: `src/core/LosShortcutManager/`

```cpp
class LosShortcutManager
{
    static LosShortcutManager& instance();
    void reg(const QString &key, QWidget *parent,
             std::function<void()> action, const QString &des = "");
};
```

---

### 1.16 其他 Core 模块

| 模块 | 位置 | 职责 |
|------|------|------|
| `LosSesssion` | `core/LosSesssion/` | 会话历史（恢复上次打开的文件/目录） |
| `LosState` | `core/LosState/` | 全局运行状态 |
| `LosPanelManager` | `core/LosPanelManager/` | 底部面板（输出/问题/终端）管理 |
| `LosPlatform` | `core/LosPlatform/` | 平台差异封装（路径/命令） |
| `LosLog` | `core/LosLog/` | 日志输出 |

---

## 2. Model 层数据结构

### 2.1 LosFileNode - 文件节点

```cpp
class LosFileNode
{
    LosFilePath LOS_filePath;          // 文件路径
    QList<LosFileNode*> LOS_childrens; // 子节点
    bool LOS_isDir;                    // 是否目录
    bool LOS_expanded;                 // 展开状态
};
```

### 2.2 LosFileTreeModel - 文件树模型

继承 `QAbstractItemModel`，为文件树视图提供 Qt Model/View 支持，并结合 `LosGitManager`
提供 Git 状态数据用于着色。

### 2.3 LosFilePath - 文件路径

封装文件路径及相关属性。

### 2.4 LosFileContext - 文件上下文

管理当前打开文件的状态（内容、光标位置、编码等）。

---

## 3. 模块间依赖关系

```
LosRouter (信号总线，贯穿全局)
    │
    ├── LosRunManager ──▶ LosAbstractRunner (各 Runner) ──▶ QProcess
    ├── LosLspManager ──▶ LosLspClient (各语言 LSP)
    ├── LosToolChainManager ──▶ (检测/下载工具)
    ├── LosConfigManager ──▶ LosConfigFactory ──▶ LosConfig*
    ├── LosNet ──▶ 插件市场 / 认证 / AI 后端
    │       └── LosAgentManager (复用 LosNet 做 AI 请求)
    ├── LosGitManager ──▶ libgit2
    ├── LosThemeManager ──▶ QSS / token
    └── LosFormatManager / LosHighlighter / LosEncodingDetector ...

Model (LosFileTreeModel / LosFileNode ...) ◀── View 读取
                                           ◀── LosGitManager 提供状态
```
