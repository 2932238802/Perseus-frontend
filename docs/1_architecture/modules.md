# 模块详解

> 本文档详细介绍 Perseus 各层核心模块的职责、API 和依赖关系。

---

## 1. Core 层模块

### 1.1 LosRunManager - 运行管理器

**文件位置**: `src/core/LosRunner/LosRunManager/`

**主要功能**: 负责编译和运行各种语言的项目/文件，根据文件类型自动选择对应的编译运行器。

**支持的语言**:

| 语言 | Runner 类 |
|------|-----------|
| C++ (单文件) | `LosSingleCppRunner` |
| CMake 项目 | `LosCmakeRunner` |
| Python | `LosPythonRunner` |
| Rust | `LosRustcRunner` |

**核心 API**:

```cpp
class LosRunManager : public QObject
{
    // 执行指定的文件或项目
    void execute(const QString &filePath, bool isProject);
    
    // 停止所有运行中的程序
    void stop();
    
    // 槽函数: 工具链就绪时回调
    void onToolChainReady(LosCommon::LosLanguage lang, 
                         LosCommon::LosTool tool, 
                         const QString &exePath);
}
```

**依赖的 Runner 抽象基类**:

```cpp
class LosAbstractRunner
{
    virtual void initConnect() = 0;
    virtual void compile() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};
```

**信号/槽接口**:

| 信号 | 说明 |
|------|------|
| `_cmd_toolChainReady` | 语言工具链就绪 |
| `_cmd_buildToolReady` | 构建工具就绪 |

---

### 1.2 LosLspManager - LSP 管理器

**文件位置**: `src/core/LosLsp/LosLspManager/`

**主要功能**: 管理多个语言的 LSP 客户端，提供代码补全、定义跳转、语义高亮等功能。

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
    // 打开文件，发送 didOpen 消息给 LSP
    void openFile(const QString &filePath, LosFileContext *fileContext);
    
    // 文本变更，发送 didChange 消息
    void changeFile(const QString &filePath, LosFileContext *fileContext);
    
    // 请求代码补全
    void requestCompletion(const QString &filePath, int line, int col);
    
    // 请求定义跳转
    void requestDefinition(const QString &filePath, int line, int col);
    
    // 请求语义高亮
    void requestSemanticTokens(const QString &filePath);
    
    // 根据文件获取对应的 LSP 客户端
    LosLspClient* getClient(const QString &filePath);
}
```

**LSP 客户端基类接口**:

```cpp
class LosLspClient : public QObject
{
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void didOpen(const QString &filePath, const QString &content) = 0;
    virtual void didChange(const QString &filePath, const QString &content) = 0;
    virtual void completion(int line, int col) = 0;
    virtual void definition(int line, int col) = 0;
};
```

---

### 1.3 LosConfigManager - 配置管理器

**文件位置**: `src/core/LosConfig/LosConfigManager/`

**主要功能**: 解析和管理项目配置文件，用于识别项目类型。

**核心 API**:

```cpp
class LosConfigManager : public QObject
{
    // 分析项目目录结构
    LosConfig* analyse(const QString &absolutePath);
    
    // 创建配置对象
    LosConfig* create(const QString &filePath);
    
    // 判断文件是否在核心文件中
    bool isInFiles(const QString &fileName);
}
```

**配置工厂**:

```cpp
class LosConfigFactory
{
    // 根据文件类型创建对应的配置解析器
    static LosConfig* create(LosFileType type);
};
```

**现有配置实现**:

| 配置类型 | 类 | 识别文件 |
|---------|-----|----------|
| CMake | `LosConfigCMake` | CMakeLists.txt |
| C++ | `LosConfigCpp` | *.cpp, *.h |
| Python | `LosConfigPython` | *.py |
| Rust | `LosConfigRust` | Cargo.toml |

---

### 1.4 LosToolChainManager - 工具链管理器

**文件位置**: `src/core/LosToolChainManager/`

**主要功能**: 从配置文件读取工具链配置，检测系统中是否已安装对应工具。

**配置文件格式**: `toolchain_config.json`

**核心 API**:

```cpp
class LosToolChainManager : public QObject
{
    // 初始化配置
    void initConfig();
    
    // 检查指定语言的工具链
    void onCheckLanguageToolchain(LosCommon::LosLanguage lang, 
                                LosCommon::LosTool tool);
    
    // 检查单个工具是否存在
    void onCheckSingleTool(const QString &tool);
}
```

**信号接口**:

| 信号 | 说明 |
|------|------|
| `_cmd_toolChainReady(lang, tool, exePath)` | 语言编译工具就绪 |
| `_cmd_buildToolReady(tool, exePath, args)` | 构建工具就绪 |
| `_cmd_lspReady(tool, exePath, args)` | LSP 工具就绪 |
| `_cmd_toolChainMissing(config)` | 工具缺失 |

---

### 1.5 LosHighlighter - 代码高亮

**文件位置**: `src/core/LosHighlighter/`

**主要功能**: Qt 语法高亮器，支持两种模式。

**支持模式**:

1. **正则表达式模式**: 基于预定义规则的简单高亮
2. **语义模式**: LSP 语义标记 (Semantic Tokens)

**语法高亮规则 (C++)**:

| 类型 | 正则/格式 | 颜色 |
|------|----------|------|
| 关键词 | `\b(if\|for\|while\|...)\b` | 粉紫 `#E17899` |
| 预处理指令 | `#\w+` | 紫色 `#B97EE6` |
| 类/类型 | `[A-Z]\w*` | 橙色 `#F5B83D` |
| 函数 | `\w+(?=\()` | 青色 `#2DCCCF` |
| 字符串 | `"[^"]*"` | 绿色 `#42E6A4` |
| 单行注释 | `//[^\n]*` | 灰色 `#5B7888` |
| 多行注释 | `\/.*\*\/` | 灰色 `#5B7888` |

**核心 API**:

```cpp
class LosHighlighter : public QSyntaxHighlighter
{
    // Qt 高亮回调 (override)
    void highlightBlock(const QString &text) override;
    
    // 更新语义标记
    void updateSemanticTokens(const QJsonObject &data);
    
    // 初始化语义图例
    void initSemanticLegend(const QJsonObject &types, 
                        const QJsonObject &modifiers);
}
```

---

### 1.6 LosFormat - 格式化模块

**文件位置**: `src/core/LosFormat/`

**主要功能**: 代码格式化。

**支持的语言**:

| 语言 | 工具 | 实现类 |
|------|------|--------|
| C++ | ClangFormat | `LosFClangFormat` |
| CMake | Neocmakelsp | `LosFNeocmakelsp` |

**核心 API**:

```cpp
class LosFormatManager
{
    // 单例获取
    static LosFormatManager* instance();
    
    // 格式化文本
    QString format(const QString &filePath, const QString &rawContent);
}
```

---

### 1.7 LosTerminal - 终端模块

**文件位置**: `src/core/LosTerminal/`

**主要功能**: 嵌入式终端模拟，基于 QProcess 实现。

**跨平台实现**:

| 平台 | 命令 |
|------|------|
| Linux | `script -q /dev/null` |
| Windows | `powershell.exe` |

**核心 API**:

```cpp
class LosTerminal : public QObject
{
    // 启动终端进程
    void onTerminalReady();
    
    // 写入数据到终端
    void write(const QString &content);
    
    // 读取标准输出
    void onReadyReadStdOut();
    
    // 读取标准错误
    void onReadyReadStdErr();
}
```

---

### 1.8 LosFileSystem - 文件系统

**文件位置**: `src/core/LosFileSystem/`

**主要功能**: 提供文件/目录操作接口。

**核心 API**:

```cpp
class LosFileSystem
{
    static LosFileSystem* instance();
    
    bool createDir(const QString &path);
    bool createFile(const QString &filePath, const QByteArray &data);
    bool removeFile(const QString &filePath);
    bool removeDir(const QString &dirPath);
    bool renameFile(const QString &src, const QString &dst);
}
```

---

### 1.9 ShortCutManager - 快捷键管理

**文件位置**: `src/core/LosShortcutManager/`

**主要功能**: 注册和管理全局快捷键。

**核心 API**:

```cpp
class LosShortcutManager
{
    static LosShortcutManager* instance();
    
    // 注册快捷键
    void reg(const QString &key, QWidget *parent, 
             std::function<void()> action, 
             const QString &des = "");
}
```

---

## 2. Model 层数据结构

### 2.1 LosFileNode - 文件节点

**文件位置**: `src/models/LosFileNode/`

**数据结构**:

```cpp
class LosFileNode
{
    LosFilePath LOS_filePath;           // 文件路径
    QList<LosFileNode*> LOS_childrens;    // 子节点列表
    bool LOS_isDir;                     // 是否目录
    bool LOS_expanded;                  // 展开状态
};
```

---

### 2.2 LosFileTreeModel - 文件树模型

**文件位置**: `src/models/LosFileTreeModel/`

**继承**: `QAbstractItemModel`

**主要功能**: 提供文件树的 Qt Model/View 架构支持。

---

### 2.3 LosFilePath - 文件路径

**文件位置**: `src/models/LosFilePath/`

**主要功能**: 封装文件路径及相关属性。

---

### 2.4 LosFileContext - 文件上下文

**文件位置**: `src/models/LosFileContext/`

**主要功能**: 管理当前打开文件的状态（内容、光标位置等）。

---

## 3. 模块间依赖关系

```
LosRouter (信号总线)
    │
    ├── LosRunManager --> LosAbstractRunner (各种 Runner)
    │
    ├── LosLspManager --> LosLspClient (各语言 LSP 客户端)
    │
    ├── LosToolChainManager
    │
    ├── LosFileSystem (文件系统操作)
    │
    └── LosHighlighter (通过 LSP 获取语义)
        │
LosConfigManager --> LosConfigFactory --> LosConfig (具体配置)
LosFormatManager --> LosFormatBase --> LosFClangFormat / LosFNeocmakelsp
LosShortcutManager (单例)
LosTerminal (单例)
```