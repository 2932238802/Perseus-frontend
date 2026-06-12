# 设计模式

> 本文档介绍 Perseus 项目中使用的主要设计模式及其在代码中的实际落地。

---

## 1. 单例模式（Meyers Singleton）

**应用场景**: 全局唯一的管理器 / 服务实例。

**实现类（实际在用）**:

| 类 | 文件 | 说明 |
|-----|------|------|
| `LosRouter` | `core/LosRouter/` | 全局信号总线 |
| `LosNet` | `core/LosNet/` | 网络请求（插件市场 / 认证 / AI） |
| `LosGitManager` | `core/LosGitManager/` | Git 仓库追踪（libgit2） |
| `LosAgentManager` | `core/LosAgent/LosAgentManager/` | AI 助手消息处理 |
| `LosThemeManager` | `core/LosTheme/` | 主题切换与 QSS 生成 |
| `LosShortcutManager` | `core/LosShortcutManager/` | 快捷键注册 |
| `LosFileSystem` | `core/LosFileSystem/` | 文件系统操作 |

**实际实现方式（Meyers Singleton — 函数内局部静态）**:

```cpp
namespace LosCore
{
    class LosAgentManager : public QObject
    {
        Q_OBJECT
    public:
        static LosAgentManager &instance()
        {
            static LosAgentManager _instance;   // C++11 起线程安全的局部静态初始化
            return _instance;
        }

        // 禁止拷贝
        LosAgentManager(const LosAgentManager &)            = delete;
        LosAgentManager &operator=(const LosAgentManager &) = delete;

    private:
        explicit LosAgentManager(QObject *parent = nullptr);
        ~LosAgentManager() = default;
    };
}
```

**关键约定**:
- 返回**引用**（`T&`），而非裸指针，调用处用 `LosXxx::instance().method()`。
- 私有构造/析构 + `= delete` 拷贝，杜绝多实例。
- 部分类用 `Q_DISABLE_COPY(LosThemeManager)` 宏达到同样效果。
- 无需手动 `delete`，进程退出时自动析构（如 `LosGitManager` 析构里 `git_libgit2_shutdown()`）。

> ⚠️ 不要再使用 "静态指针 + new" 的旧式单例写法，统一用上面的 Meyers Singleton。

---

## 2. 工厂模式

**应用场景**: 根据类型创建对应对象。

### 2.1 配置工厂 - LosConfigFactory

**文件位置**: `src/core/LosConfig/LosConfigFactory/`

根据项目类型创建对应的配置解析器：

```cpp
class LosConfigFactory
{
public:
    static LosConfig* create(LosFileType type)
    {
        switch (type) {
            case LosFileType::CMake:  return new LosConfigCMake();
            case LosFileType::Cpp:    return new LosConfigCpp();
            case LosFileType::Python: return new LosConfigPython();
            case LosFileType::Rust:   return new LosConfigRust();
            default:                  return nullptr;
        }
    }
};
```

### 2.2 Runner 工厂

`LosRunManager` 根据文件 / 项目类型选择具体 Runner：

```
LosRunManager.execute(filePath, isProject)
    │
    ▼ 根据文件类型 / 是否项目
    ├── LosSingleCppRunner (C++ 单文件)
    ├── LosCmakeRunner     (CMake 项目)
    ├── LosPythonRunner    (Python)
    └── LosRustcRunner     (Rust)
```

---

## 3. 抽象（接口）基类

### 3.1 Runner 抽象基类

```cpp
class LosAbstractRunner : public QObject
{
public:
    virtual void initConnect() = 0;
    virtual void compile()     = 0;
    virtual void run()         = 0;
    virtual void stop()        = 0;
};
```

### 3.2 LSP 客户端抽象基类

```cpp
class LosLspClient : public QObject
{
public:
    virtual void initialize() = 0;
    virtual void shutdown()   = 0;
    virtual void didOpen(const QString &filePath, const QString &content) = 0;
    virtual void didChange(const QString &filePath, const QString &content) = 0;
    virtual void completion(int line, int col) = 0;
    virtual void definition(int line, int col) = 0;
};
```

---

## 4. 观察者模式（信号 / 槽 + 信号总线）

**应用场景**: 模块间松耦合通信，核心载体是 **LosRouter** 信号总线。

```cpp
// 注册观察者
connect(&LosCore::LosRouter::instance(),
        &LosCore::LosRouter::_cmd_gotoFile,
        this, &ThisClass::onGotoFile);

// 发出通知
emit LosCore::LosRouter::instance()._cmd_gotoFile(filePath, line);
```

LosRouter 集中声明全部信号（LSP / 文件 / 工具链 / 网络 / 认证 / AI / Git / 主题 / 工具栏），
各模块只依赖 LosRouter，互不直接引用，实现解耦。详见
[`overview.md` §4](./overview.md)。

---

## 5. 命令模式

**应用场景**: 命令面板（`Ctrl+Shift+P`）的命令注册与执行，以及插件命令扩展。

```cpp
// 命令注册（id, 显示名, 快捷键, 回调）
LosCommandUi::regis("myExtension.hello", "Hello 命令", "Ctrl+Shift+H",
                    []() {
                        qDebug() << "Hello from plugin!";
                    });
```

命令对象将「请求」封装为可注册、可检索、可延迟执行的单元，支持插件动态注入。

---

## 6. 策略模式

### 6.1 格式化策略

`LosFormatManager` 按语言选择不同格式化后端：

```cpp
QString LosFormatManager::format(const QString &filePath, const QString &content)
{
    auto lang = CheckLang::getLanguage(filePath);
    if (lang == LosLanguage::CXX)
        return LosFClangFormat::format(content);          // clang-format
    if (lang == LosLanguage::CMake)
        return LosFNeocmakelsp::format(filePath, content);// neocmakelsp
    return content;
}
```

### 6.2 主题策略

`LosThemeManager` 根据主题名加载对应 token 表，注入 QSS 模板：

```cpp
QString qss = LosThemeManager::instance().buildMainQss("dracula");
LosThemeManager::instance().setTheme("dracula");   // 切换并广播 _cmd_themeChanged
```

---

## 7. 模板方法模式

Runner / LSP 客户端基类定义算法骨架（`initConnect → compile → run → stop`、
`initialize → didOpen → didChange → ...`），具体子类实现各步骤细节，
保证流程统一、扩展点清晰。

---

## 8. 桥接 / 适配（JS ↔ Qt）

终端 `LosTerminalUi` 通过 **QWebChannel** 在 xterm.js（JavaScript）与 Qt（C++）之间
搭桥，把前端按键事件转发给后端 PTY，把后端输出回写到前端，属于典型的适配 / 桥接思路。
