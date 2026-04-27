# Perseus — AI 上下文总览（AI Context Digest）

> 本文件是 **面向 AI 代理（Claude / Cursor / Copilot / Gemini / OpenCode 等）** 的单文件全景摘要。
> 目的：
> 1. **全文分析** — AI 读一次即可掌握项目全貌、术语体系、模块边界
> 2. **决策与设计** — 提供做架构决策 / 写新功能 / 重构 时的判据与约束
> 3. **书写与命名规范** — 让 AI 产出的代码直接与仓库现有风格对齐
>
> 其它文档（README.md / CLAUDE.md / AGENTS.md / docs/1_architecture/ 等）为权威来源；
> **凡出现冲突，以本文件标注的「权威源」为准，不要自行推断。**

---

## 0. TL;DR（给 AI 的 30 秒摘要）

- **身份**：Perseus 是一款仿 VSCode 的**桌面 IDE**，用 **C++20 + Qt 6** 编写，跑在 **Ubuntu / WSL2**。
- **构建**：`CMake ≥ 3.18` + `Ninja` + `Clang-18`，入口见 `CMakeLists.txt`（项目实际编译为 `C++20`，注意 README 仍写 C++17，**权威源是 CMakeLists.txt L14**）。
- **目标产物**：静态库 `PerseusCore` + 可执行文件 `Perseus`（入口 `src/main.cpp`，主窗口 `src/Perseus.{h,cpp}`）。
- **分层**：`view` / `core` / `models` / `common` 四层，严禁倒置；跨层通信一律走 `LosRouter` 信号总线。
- **命名前缀**：类用 `Los` 前缀；成员用 `L_`（普通）/ `LOS_`（持有的服务对象）前缀；**不是 `m_`**。
- **禁忌**：业务逻辑写进 `view/`、view 直接调 core service、裸 `new/delete`、阻塞主线程、Tab 缩进、异常穿越 Qt 事件循环。

---

## 1. 技术栈与版本（权威源：`CMakeLists.txt`）

| 类别 | 规格 | 备注 |
|------|------|------|
| 语言 | **C++20**（`CMAKE_CXX_STANDARD 20`，L14） | README/AGENTS 旧文档写 C++17，以 CMake 为准 |
| UI 框架 | Qt 6.x（`Widgets / Network / Concurrent / WebEngineWidgets / WebChannel / Test / Core5Compat`） | WebEngine 为 xterm.js 终端用 |
| 构建 | CMake ≥ 3.18 + Ninja | `CMAKE_EXPORT_COMPILE_COMMANDS=ON` |
| 编译器 | Clang-18（`/usr/bin/clang++-18`） | 脚本固定，不要随意改 GCC |
| 平台 | Ubuntu 24.04 / WSL2（WSLg GUI） | Windows 原生/mac 未支持 |
| LSP 后端 | clangd（C++）、rust-analyzer（Rust）、neocmakelsp（CMake）、pyright（Python） | |
| 终端栈 | xterm.js + QWebChannel + QWebEngineView（Linux 走 `script -q`，Win 走 `powershell.exe`） | |
| 测试 | Qt Test + CTest，源文件 `test/LosTest.cpp`，目标名 `LosTest` | 链接到 `PerseusCore` |

---

## 2. 目录结构与"放哪里"速查

```
Perseus-frontend/
├── src/
│   ├── main.cpp                  # 入口
│   ├── Perseus.{h,cpp,ui?}      # 根 QMainWindow，持有并串联所有核心管理器
│   ├── common/                   # 常量 / 枚举 / 工具函数（无业务）
│   │   ├── constants/  enum/  util/
│   ├── core/                     # 业务与服务（无 UI）
│   │   ├── LosConfig/           # 项目类型识别（CMake/Cpp/Py/Rust）
│   │   ├── LosFileSystem/       # 文件系统操作
│   │   ├── LosFormat/           # 代码格式化（ClangFormat / neocmakelsp）
│   │   ├── LosHighlighter/      # 语法+语义高亮
│   │   ├── LosLog/              # 日志（INF/WAR/ERR/SUC 宏）
│   │   ├── LosLsp/              # LSP 客户端集合（一语言一客户端）
│   │   ├── LosNet/              # 网络（插件市场）
│   │   ├── LosPlatform/         # 平台差异
│   │   ├── LosRouter/           # ★ 全局信号总线（单例）
│   │   ├── LosRunner/           # 编译运行（单文件 / CMake / Py / Rust）
│   │   ├── LosSesssion/         # 会话恢复（注意拼写：3 个 s）
│   │   ├── LosShortcutManager/  # 快捷键
│   │   ├── LosState/            # ★ 线程安全全局 KV 状态
│   │   ├── LosTerminal/         # 终端（QProcess 包 script/pwsh）
│   │   └── LosToolChainManager/ # 工具链探测与配置
│   ├── models/                   # Qt Model / 数据结构（无业务）
│   │   ├── LosFileContext/  LosFileNode/  LosFilePath/  LosFileTreeModel/
│   └── view/                     # UI 控件 + .ui 文件（无业务）
│       ├── Perseus.ui  style/   # 主布局 + QSS
│       ├── LosEditorTabUi/  LosEditorUi/  LosLineNumberUi/  LosCompleterUi/
│       ├── LosFileTreeUi/  LosTerminalTabsUi/  LosTerminalUi/
│       ├── LosCommandUi/  LosCommandArgsUi/  LosDropdownButtonUi/  LosFloatingPanelUi/
│       ├── LosIssuesUi/  LosPlugUi/  LosPluginDetailUi/  LosSettingsUi/  LosToolMissUi/
├── resources/  resources.qrc     # 图标、xterm.js 等
├── docs/                         # 详细文档（见 §9 导航）
├── test/                         # Qt Test
├── tools/                        # 周边脚本
├── CMakeLists.txt                # 构建权威源
├── env.sh / re-build.sh / build.sh / gdb-build.sh / clean.sh / makefile
├── .clang-format                 # 格式权威源（CI 强制）
├── AGENTS.md / CLAUDE.md / README.md / README_Gemini_Context.md
```

### 放哪里（AI 决策表）

| 需求 | 落位目录 | 不能落在 |
|------|---------|---------|
| 新增 UI 控件 / 对话框 | `src/view/LosXxxUi/` + 对应 `.ui` | core / models |
| 新增业务服务 / 管理器 | `src/core/LosXxx/` | view（UI 层） |
| 新增数据结构 / Qt Model | `src/models/LosXxx/` | core（除非纯业务对象） |
| 新常量 / 枚举 / 小工具函数 | `src/common/{constants,enum,util}/` | 其他层 |
| 新语言 Runner | `src/core/LosRunner/` 继承 `LosAbstractRunner` | 直接改 `LosRunManager` switch |
| 新语言 LSP | `src/core/LosLsp/` 继承 `LosLspClient` | 同上 |
| 新项目类型识别 | `src/core/LosConfig/` + 在 `LosConfigFactory` 注册 | view |
| 新格式化策略 | `src/core/LosFormat/` 继承 `LosFormatBase` | view |

---

## 3. 架构原则（硬性约束）

### 3.1 分层与依赖方向（单向，禁止反向）

```
View   ───▶  Core     （UI 发意图，不直接调 Core API — 发信号）
View   ───▶  Model    （读数据）
Core   ───▶  Model    （改数据）
Core   ───▶  Core     （互调 OK）
Common ◀──   所有层    （只被依赖）
```

**铁律**：
1. `view/*` 里**禁止**出现业务逻辑（算编译命令、起 QProcess、拼 LSP JSON-RPC 都不行）。
2. `view/*` **禁止**直接持有 `core/*` 的服务对象并调用其非信号方法；要通信 → `emit LosRouter::instance()->_cmd_xxx(...)`。
3. `models/*` 只有数据与 Qt Model 接口，**不准起异步、不准访问文件系统、不准 QNetworkAccessManager**（文件节点本身除外）。
4. `common/*` 是纯工具，**不得依赖** `core / models / view` 中任何类型。

### 3.2 通信：`LosRouter` 信号总线（外观模式 + 观察者）

- 单例，`LosRouter::instance()`
- 所有跨组件事件/命令都以 **Qt signal** 的形式挂在 `LosRouter` 上
- 命名前缀见 §5.3

```cpp
// 发出
emit LosRouter::instance()->_cmd_gotoFile(path, line);

// 监听
connect(LosRouter::instance(), &LosRouter::_cmd_gotoFile,
        this, &LosEditorTabUi::openAt);
```

### 3.3 全局状态：`LosState`

- 线程安全 KV：`QHash<SG_STR, QVariant>`
- 用于跨模块只读/只写很少的全局值，例如 `SG_STR::PROJECT_DIR`
- **不要**把频繁变化的 UI 状态塞进来，那是 model 的事

### 3.4 线程模型

- 主线程 = Qt 事件循环 = UI，**不得阻塞**
- 编译/运行/LSP 一律走 `QProcess` 异步；耗时 CPU 任务用 `QtConcurrent` / 线程池
- Qt 信号槽跨线程默认 `QueuedConnection`，别手动 `invokeMethod` 搞花活

---

## 4. 关键子系统速览（AI 做设计前先读这里）

### 4.1 运行体系（`LosRunner/`）

- `LosRunManager`：入口，根据 `(filePath, isProject)` 分派到具体 Runner
- `LosAbstractRunner`（纯虚）：`initConnect() / compile() / run() / stop()`，**模板方法模式**
- 具体：`LosSingleCppRunner` / `LosCmakeRunner` / `LosPythonRunner` / `LosRustcRunner`
- 扩展新语言 = 新增一个 `LosXxxRunner` 子类 + 在分派处注册，**不改调用方**

### 4.2 LSP 体系（`LosLsp/`）

- `LosLspManager`：持有每语言一个 `LosLspClient`（子进程 + JSON-RPC）
- 就绪前的请求塞 `L_pendings` 队列，就绪后再 flush
- 客户端契约：`initialize / shutdown / didOpen / didChange / completion / definition`
- 新增语言 LSP = 继承 `LosLspClient`，在 `LosLspManager` 按文件扩展名映射

### 4.3 工具链（`LosToolChainManager/`）

- 读 `toolchain_config.json`，探测系统可执行
- 发出 `_cmd_toolChainReady / _cmd_buildToolReady / _cmd_lspReady / _cmd_toolChainMissing`
- **任何依赖外部可执行文件的功能**都应等这些信号就绪后再起

### 4.4 编辑器（`view/LosEditorUi/`）

- `LosEditorUi` 继承 `QPlainTextEdit`
- 组合：`LosLineNumberUi`（行号栏）+ `LosHighlighter`（高亮）+ `LosCompleterUi`（补全）+ hover + diagnostics
- 多标签由 `LosEditorTabUi` 管理：`path → LosEditorUi*` 映射

### 4.5 终端（`LosTerminal/` + `view/LosTerminalUi/`）

- 内部 `QProcess` 驱动 PTY（Linux `script -q /dev/null` / Win `powershell.exe`）
- UI 侧用 `QWebEngineView` 加载 xterm.js，双向通过 `QWebChannel` 传输字节流

### 4.6 配置识别（`LosConfig/`）

- `LosConfigManager.analyse(dir)` → `LosConfigFactory::create(type)` → 具体 `LosConfigXxx`
- 识别到后触发对应语言的 LSP workspace 初始化

### 4.7 会话（`LosSesssion/`，注意拼写）

- 打开文件列表、活动 tab、项目目录持久化；下次启动恢复

### 4.8 插件（`view/LosPlugUi/` + `core/LosNet/`）

- 现阶段 = 插件市场浏览 + `package.json` 清单（命令 / 快捷键贡献点）
- 具体 runtime API 见 `docs/2_extensions/`

---

## 5. 编码与命名规范（**AI 生成代码时必须一致**）

### 5.1 命名（**权威：CLAUDE.md**，覆盖 AGENTS.md 的 `m_` 描述）

| 实体 | 规则 | 示例 |
|------|------|------|
| 类 / 结构体 | `PascalCase`，项目类型前缀 `Los` | `LosRunManager`, `LosFilePath` |
| 方法 / 函数 | `camelCase` | `initConnect()`, `onReadyRead()` |
| **普通成员变量** | **`L_` 前缀** + `camelCase` | `L_timer`, `L_pendings`, `L_dirty` |
| **持有的服务对象成员** | **`LOS_` 前缀** + `camelCase` | `LOS_runMgr`, `LOS_tabUi` |
| 局部变量 / 参数 | `camelCase`，无前缀 | `filePath`, `lineNo` |
| 常量 / 宏 | `UPPER_SNAKE_CASE` | `MAX_RETRIES`, `DEFAULT_TIMEOUT` |
| 命名空间 | `LosView / LosCore / LosModel / LosCommon` | |
| 文件夹 / .h / .cpp | 与类同名 | `LosRunManager/LosRunManager.h` |

> ⚠️ **绝不要**用 `m_` / `g_` / `s_` 前缀；AGENTS.md 中旧文写法作废。

### 5.2 格式化（**权威：`.clang-format`，CI 强制**）

- LLVM base，**列宽 120**，**4 空格缩进**（禁 Tab）
- **Allman** 大括号（大括号另起一行）
- 指针**右对齐**：`Type *ptr`
- `#include` 按块字典序排序
- **每次改完必须**：`clang-format -i path/to/file.{cpp,h}`

### 5.3 信号命名

| 前缀 | 语义 | 示例 |
|------|------|------|
| `_cmd_` | 普通命令/事件 | `_cmd_gotoFile`, `_cmd_fileDirty` |
| `_cmd_lsp_request_` | View → LSP 请求 | `_cmd_lsp_request_openFile` |
| `_cmd_lsp_result_` | LSP → View 结果 | `_cmd_lsp_result_completion` |
| `_cmd_toolChainXxx` | 工具链事件 | `_cmd_toolChainReady` |

所有新增信号挂到 `LosRouter`，不要在 view/core 自己的类上另起私有总线。

### 5.4 头文件与 include 顺序

1. 对应的自家 `.h`（`Foo.cpp` 首行 `#include "Foo.h"`）
2. 项目内头文件（`"core/..."`, `"models/..."`）
3. Qt 头文件（`<QString>` …）
4. 标准库头文件（`<memory>` …）

`.h` 里**优先前置声明**；只有 `.cpp` 才 include 完整依赖。

### 5.5 内存与所有权

- **`QObject` 派生**：构造时传 `parent`，让 Qt 对象树回收，**不要**对它们用 `unique_ptr`
- **非 `QObject`**：`std::unique_ptr` / `std::shared_ptr`
- **禁止裸 `new/delete`**，除非立刻交给 Qt parent 或智能指针

### 5.6 错误处理

- **早返回**，禁止多层嵌套；返回前 `qWarning()` 或 `ERR(...)`
- **返回 `bool` / `std::optional`**，不要在 Qt 事件循环中抛异常
- 网络/进程必连 `errorOccurred` 与 `finished`；**永远不假设成功**

### 5.7 Qt / UI 约定

- 静态布局走 `.ui` 文件 + `setupUi(this)`；动态才用代码搭
- UI 层**禁止**持有 `core` 服务指针并调用其业务方法 → 发 `LosRouter` 信号
- 任何耗时操作必须异步 / `QtConcurrent`，绝不在主线程阻塞

### 5.8 容器与类型

- 和 Qt 交互优先 Qt 容器（`QString / QList / QVector / QHash`）
- `core` 纯计算可用 `std::` 容器
- 网络 / 序列化用定长整型（`qint32`, `int32_t`）
- 显然场景可 `auto`，否则显式类型

### 5.9 注释

- 命名自解释，注释只写**为什么**，不写**是什么**
- 死代码立删，不留 `// TODO 以后再说`

---

## 6. 构建 / 测试 / 格式化命令（别自创）

| 场景 | 命令 | 说明 |
|------|------|------|
| 首次 / 彻底重建 | `./re-build.sh` | 删 `_build`，CMake + Ninja + Release |
| 日常增量 | `./build.sh` | 只增量编译并运行 |
| Debug 构建 | `./gdb-build.sh` | 带调试符号 |
| 仅运行 | `make run` 或 `./_build/Perseus` | |
| GDB 调试 | `make gdb` | |
| 清理 | `./clean.sh` | 删 `_build` |
| 安装依赖 | `./env.sh` | 幂等，可重复跑 |
| 全部测试 | `cd _build && ctest --output-on-failure` | |
| 单测 | `cd _build && ctest -R <Name> -V` | |
| 构建测试目标 | `cmake --build _build --target LosTest` | |
| 格式化 | `clang-format -i <files>` | **提交前必须跑** |
| 代码行数 | `./test/loc.sh` | 分模块统计 |

---

## 7. 设计模式地图（遇到扩展需求优先沿用现有模式）

| 模式 | 位置 / 用途 |
|------|-----------|
| 单例 | `LosRouter` / `LosState` / `LosLog` / `LosNet` / `LosFileSystem` / `LosFormatManager` / `LosShortcutManager` |
| 工厂 | `LosConfigFactory`，Runner 分派 |
| 抽象基类（抽象工厂） | `LosAbstractRunner`, `LosLspClient`, `LosFormatBase` |
| 模板方法 | `LosAbstractRunner::execute()`（固定 initConnect → compile → run） |
| 观察者 | Qt 信号槽 + `LosRouter` |
| 策略 | `LosFormatManager` 按语言分派 ClangFormat / neocmakelsp |
| 命令 | 命令面板 `Command { id, label, keybinding, handler }` |
| 外观 | `LosRouter` 统一对外信号出口 |

**新增功能决策流程**（AI 请按此顺序自问）：

1. 这是 UI 还是业务？→ 决定放 `view` 还是 `core`
2. 能否复用已有抽象（`LosAbstractRunner` / `LosLspClient` / `LosFormatBase` / `LosConfig`）？
3. 跨组件通信 → 在 `LosRouter` 加一个 `_cmd_xxx` 信号，别建新的单例
4. 需要外部进程 → `QProcess` 异步 + 依赖工具链就绪信号
5. 需要持久化 → 走 `LosSesssion` 或 `LosState`，不要自写 JSON 散落到处

---

## 8. AI 协作时的"红线与最佳实践"

### 红线（不要做）

- ❌ 在 `view/` 里 `new QProcess`、拼命令行、读写项目文件
- ❌ view 直接 `include` 并调用 `core` 的管理器业务方法（只能 connect 信号）
- ❌ 用 `m_ / g_ / s_` 等非本项目前缀
- ❌ 改构建脚本使用 GCC 或切换 Make 生成器
- ❌ 提交未 `clang-format` 的代码
- ❌ 在 Qt 事件循环里抛异常、或让主线程 sleep/同步 IO
- ❌ 自建第二个信号总线 / 单例注册中心
- ❌ 为 `QObject` 派生类使用 `std::unique_ptr`（应该走 Qt parent 树）
- ❌ 删除 / 重命名既有 `LosRouter` 信号而不搜调用点（会静默断联）

### 最佳实践（这样做）

- ✅ 新服务继承自合适的抽象基类；新交互先加一条 `LosRouter` 信号
- ✅ 外部进程 / 网络**全异步**，失败路径要有日志（`ERR("reason", "Component")`）
- ✅ 涉及文件 / 目录路径时使用 `LosFilePath` / `LosFileSystem`，不要散用 `QFile`
- ✅ 编辑器相关先看 `LosEditorUi` / `LosEditorTabUi`，沿用现有组合方式
- ✅ 给 AI 生成补丁：**先列出会动的文件 + 分层归属 + 新增信号名**，再写代码
- ✅ 修改公共头要 `re-build.sh`；只改 `.cpp` 用 `build.sh`
- ✅ 拼写注意：目录就是 `LosSesssion`（3 个 s），不要"修正"成 `LosSession`
- ✅ 文档歧义时，优先级：`CMakeLists.txt` > `.clang-format` > `CLAUDE.md` > `docs/` > `AGENTS.md` > `README.md`

---

## 9. 深度文档导航

| 主题 | 入口 |
|------|------|
| 构建 / 环境 / 脚本细节 | [`docs/3_development/setup.md`](./3_development/setup.md) |
| 分层架构与模块关系 | [`docs/1_architecture/overview.md`](./1_architecture/overview.md) |
| 各核心模块 API | [`docs/1_architecture/modules.md`](./1_architecture/modules.md) |
| 设计模式用法 | [`docs/1_architecture/design-patterns.md`](./1_architecture/design-patterns.md) |
| 插件协议 | [`docs/2_extensions/protocol.md`](./2_extensions/protocol.md) |
| 插件开发流程 | [`docs/2_extensions/development.md`](./2_extensions/development.md) |
| Git / 提交规范 | [`docs/3_development/git-workflow.md`](./3_development/git-workflow.md) |
| 快捷键清单 | [`docs/4_ui_ux/shortcuts.md`](./4_ui_ux/shortcuts.md) |
| UI 组件 | [`docs/4_ui_ux/components.md`](./4_ui_ux/components.md) |
| 主题 | [`docs/4_ui_ux/theme.md`](./4_ui_ux/theme.md) |
| AI 工作规范（本文补充） | [`CLAUDE.md`](../CLAUDE.md), [`AGENTS.md`](../AGENTS.md) |

---

## 10. 已知文档歧义 / 陷阱（AI 请注意）

| 议题 | 不一致源 | 权威裁定 |
|------|---------|---------|
| C++ 标准 | README / AGENTS 写 C++17；CLAUDE / CMakeLists 写 C++20 | **C++20**（`CMakeLists.txt` L14） |
| 成员变量前缀 | AGENTS.md 泛泛说 `m_` | **`L_` / `LOS_`**（CLAUDE.md + 实际代码） |
| 会话目录拼写 | 常被误写成 `LosSession` | 实际为 `LosSesssion`（3 个 s） |
| 设计模式文档中的"单例骨架" | 用了 `m_instance` 作为示例成员 | 新写单例请按项目规范用 `L_instance` |
| 项目许可证 / 最终平台 | 仓库未显式写 LICENSE，跨平台支持以脚本为准 | Linux / WSL2 为主；Win/mac 无保证 |

---

## 11. AI 答复/补丁前的自检清单

- [ ] 改动在哪一层？是否破坏单向依赖？
- [ ] 是否引入了新的 `m_` 前缀 / Tab 缩进 / K&R 大括号？
- [ ] 新组件是否该继承某个已有抽象（`LosAbstract*`）？
- [ ] 跨组件通信是否经过 `LosRouter`？新增信号名是否匹配 `_cmd_` 前缀规则？
- [ ] 是否有可能阻塞主线程？外部进程/网络是否接了 `errorOccurred`？
- [ ] `QObject` 是否带 parent？非 QObject 是否用智能指针？
- [ ] include 顺序、前置声明是否合规？
- [ ] 是否跑过 `clang-format`？是否至少 `./build.sh` 通过？
- [ ] 如果动了公共头或 CMake，是否提示用户 `./re-build.sh`？
- [ ] 是否写了 `test/` 下的用例覆盖核心逻辑？

---

*本文件为 AI 上下文摘要；遇到本文未覆盖或与权威源冲突的内容，请回溯 §9 的原始文档，并在输出中明确指出引用位置（如 `src/core/LosRunner/LosRunManager.cpp:42`）。*
