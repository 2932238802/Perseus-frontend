# 设计模式

> 本文档介绍 Perseus 项目中使用的主要设计模式。

---

## 1. 单例模式

**应用场景**: 全局唯一的管理器实例

**实现类**:

| 类 | 说明 |
|-----|------|
| `LosRouter` | 信号总线全局唯一 |
| `LosFileSystem` | 文件系统操作全局唯一 |
| `LosFormatManager` | 格式化全局唯一 |
| `LosShortcutManager` | 快捷键全局唯一 |

**实现方式**:

```cpp
class LosXXXManager
{
private:
    static LosXXXManager *m_instance;
    
public:
    static LosXXXManager* instance()
    {
        if (!m_instance) {
            m_instance = new LosXXXManager();
        }
        return m_instance;
    }
};
```

---

## 2. 工厂模式

**应用场景**: 根据不同类型创建对应对象

### 2.1 配置工厂 - LosConfigFactory

**文件位置**: `src/core/LosConfig/LosConfigFactory/`

**功能**: 根据项目类型创建对应的配置解析器

```cpp
class LosConfigFactory
{
public:
    static LosConfig* create(LosFileType type)
    {
        switch (type) {
            case LosFileType::CMake:
                return new LosConfigCMake();
            case LosFileType::Cpp:
                return new LosConfigCpp();
            case LosFileType::Python:
                return new LosConfigPython();
            case LosFileType::Rust:
                return new LosConfigRust();
            default:
                return nullptr;
        }
    }
};
```

### 2.2 Runner 工厂

**功能**: 根据语言类型创建对应的 Runner

```
LosRunManager.execute(filePath, isProject)
    │
    ▼
根据文件类型选择 Runner
    │
    ├── LosSingleCppRunner (C++ 单文件)
    ├── LosCmakeRunner (CMake 项目)
    ├── LosPythonRunner (Python)
    └── LosRustcRunner (Rust)
```

---

## 3. 抽象工厂模式

### 3.1 Runner 抽象基类

```cpp
class LosAbstractRunner : public QObject
{
public:
    virtual void initConnect() = 0;
    virtual void compile() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};
```

### 3.2 LSP 客户端抽象基类

```cpp
class LosLspClient : public QObject
{
public:
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void didOpen(const QString &filePath, 
                        const QString &content) = 0;
    virtual void didChange(const QString &filePath, 
                          const QString &content) = 0;
    virtual void completion(int line, int col) = 0;
    virtual void definition(int line, int col) = 0;
};
```

---

## 4. 观察者模式

**应用场景**: 信号/槽机制实现模块间通讯

### 4.1 LosRouter 信号总线

作为观察者中心，管理所有信号的注册和分发：

```cpp
// 注册观察者
connect(LosRouter::instance(), &LosRouter::xxxSignal,
        this, &ThisClass::xxxSlot);

// 发出通知
emit LosRouter::instance()->xxxSignal(params);
```

---

## 5. 命令模式

**应用场景**: 命令面板的命令注册和执行

### 5.1 命令结构

```cpp
struct Command
{
    QString id;              // 命令 ID
    QString label;            // 显示名称
    QString keybinding;       // 快捷键绑定
    std::function<void()> handler;  // 执行回调
};
```

### 5.2 命令注册

```cpp
LosCommandUi::regis("myExtension.hello", "Hello 命令", 
                    "Ctrl+Shift+H", 
                    []() {
    qDebug() << "Hello from plugin!";
});
```

---

## 6. 策略模式

### 6.1 格式化策略

根据语言选择不同的格式化策略：

```cpp
class LosFormatManager
{
public:
    QString format(const QString &filePath, 
                  const QString &content)
    {
        auto lang = CheckLang::getLanguage(filePath);
        
        if (lang == LosLanguage::CXX) {
            return LosFClangFormat::format(content);
        } else if (lang == LosLanguage::CMake) {
            return LosFNeocmakelsp::format(filePath, content);
        }
        return content;
    }
};
```

---

## 7. 模板方法模式

**应用场景**: Runner 的编译运行流程

```cpp
class LosAbstractRunner
{
public:
    // 模板方法
    void execute()
    {
        initConnect();   // 初始化连接
        compile();      // 编译
        run();         // 运行
    }
    
    virtual void initConnect() = 0;
    virtual void compile() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};
```

子类实现具体步骤：

- `LosSingleCppRunner::compile()` - 调用 g++ 编译
- `LosCmakeRunner::compile()` - 调用 cmake 构建
- `LosPythonRunner::run()` - 直接运行 python

---

## 8. 外观模式

**应用场景**: LosRouter 统一管理各种信号

```cpp
class LosRouter : public QObject
{
    Q_OBJECT
    
public:
    // 统一的信号发送接口
    void sendToShell(const QString &text);
    void gotoFile(const QString &file, int line);
    void fileDirty(const QString &path, bool dirty);
    
signals:
    _cmd_sendToShell(QString);
    _cmd_gotoFile(QString, int);
    _cmd_fileDirty(QString, bool);
};
```

---

## 9. 设计模式使用总结

| 模式 | 使用位置 |
|------|----------|
| 单例模式 | LosRouter, LosFileSystem, LosFormatManager |
| 工厂模式 | LosConfigFactory, LosRunnerFactory |
| 抽象工厂 | LosAbstractRunner, LosLspClient |
| 观察者模式 | 信号/槽机制、LosRouter |
| 命令模式 | 命令面板 |
| 策略模式 | 格式化模块 |
| 模板方法 | Runner 执行流程 |
| 外观模式 | LosRouter 信号总线 |