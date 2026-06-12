# Agent Instructions for Perseus

This file contains essential context, build instructions, and coding standards for any AI coding agents (like Cursor, Copilot, or CLI agents) operating within the `Perseus` repository.


## 1. 项目概览与文档导航

**Perseus** 是基于 **C++23** 和 Qt 6 的现代 IDE，采用 CMake + Ninja + Clang-18 构建，支持多语言 LSP、插件扩展、AI 助手、Git 集成、跨平台终端等。

> `CMakeLists.txt` 中 `set(CMAKE_CXX_STANDARD 23)`；用到 `std::stacktrace`（链接 `stdc++exp`）。
> Qt 组件：Widgets / Network / Concurrent / WebEngineWidgets / WebChannel / Test / Core5Compat。

**核心文档导航（务必优先查阅，避免重复内容）：**

- [README.md](../README.md)：项目简介、特性、环境要求、分支/提交规范
- [docs/README.md](README.md)：文档中心总览、目录结构、核心能力速览
- [1_architecture/overview.md](1_architecture/overview.md)：分层架构、信号总线（LosRouter）、核心流程
- [1_architecture/design-patterns.md](1_architecture/design-patterns.md)：Meyers 单例、工厂、观察者等设计模式
- [1_architecture/modules.md](1_architecture/modules.md)：各核心模块 API/职责
- [3_development/setup.md](3_development/setup.md)：开发环境搭建、构建脚本说明
- [3_development/libgit2-integration.md](3_development/libgit2-integration.md)：libgit2 submodule 集成
- [2_extensions/development.md](2_extensions/development.md)：插件开发流程
- [4_ui_ux/components.md](4_ui_ux/components.md)：UI 组件清单

**目录结构简述：**
- `src/Perseus.{h,cpp}`：主窗口（QMainWindow），装配各模块
- `src/view/`：UI 组件、.ui 表单、视图逻辑（含 AgentUi / AuthUi / Preview 等）
- `src/core/`：业务逻辑、管理器、LSP、Runner、网络（LosNet）、AI（LosAgent）、Git（LosGitManager）、主题（LosTheme）、信号总线（LosRouter）
- `src/models/`：数据结构（文件树/节点/路径/上下文）
- `src/common/`：常量、工具、日志
- `resources/`：Qt 资源文件
- `third_party/`：第三方依赖源码（git submodule，与 `src/` 物理隔离，不参与 clang-format/clang-tidy）
  - `third_party/libgit2`：libgit2 v1.9.4，静态链接，用于本地 Git 仓库追踪

> ⚠️ **clone 项目后必须初始化子模块**：`git submodule update --init --recursive`，否则 `third_party/libgit2` 为空，CMake 配置会失败。

**分层原则：** 业务逻辑严禁进入 UI 层，所有跨层通信通过信号（LosRouter）完成。

---

## 2. Build, Run, and Test Commands


**务必使用如下标准命令，勿自行假设构建流程。详细说明见 [setup.md](docs/3_development/setup.md)：**

### Build & Run

- **首次构建/完整重建（Release）**：`./re-build.sh`
- **增量编译并运行（Release）**：`./rbuild.sh`
- **增量编译并运行（Debug）**：`./bbuild.sh`
- **Debug 完整重建（配合 GDB）**：`./gdb-build.sh`
- **清理**：`./clean.sh`
- **仅运行已编译产物**：`./_build/Perseus`

### Testing

**测试相关命令（详细见 [setup.md](docs/3_development/setup.md)）：**
- 运行全部测试：`cd _build && ctest --output-on-failure`
- 运行单测：`cd _build && ctest -R <TestName> -V`
- 构建指定测试目标：`cmake --build _build --target <TestTargetName>`

---

## 3. Linting and Formatting


**强制使用 `.clang-format`，CI 检查不通过会拒绝合入。**

- 基于 LLVM，120 列宽，4 空格缩进，Allman 大括号风格，指针右对齐
- 文件内 include 顺序自动排序
- 禁止 Tab

**所有代码提交前必须执行：**
```bash
clang-format -i path/to/file.cpp path/to/file.h
```

---

## 4. Code Style Guidelines

### Naming Conventions
- **Classes & Structs:** `PascalCase` (e.g., `UserModel`, `NetworkManager`).
- **Functions & Methods:** `camelCase` (e.g., `loadData()`, `parseResponse()`).
- **Variables:** `camelCase` (e.g., `requestPayload`, `itemCount`).
- **Member Variables:** Prefix with `L_` to distinguish from local variables (e.g., `L_apiClient`, `L_retryCount`).
- **Constants & Macros:** `UPPER_SNAKE_CASE` (e.g., `MAX_RETRIES`, `DEFAULT_TIMEOUT`).

> ⚠️ Perseus 代码实际采用 `L_`/`LOS_` 前缀（如 `L_timer`、`LOS_runMgr`），**与部分通用 C++ 规范（如 m_）不同**。详细见下表与 [modules.md](1_architecture/modules.md)。

| 类型 | 规范 | 示例 |
|------|------|------|
| 类/结构体 | PascalCase | LosRunManager |
| 方法/函数 | camelCase | initConnect() |
| 普通成员变量 | L_ 前缀 | L_timer |
| 服务对象成员 | LOS_ 前缀 | LOS_runMgr |
| 常量/宏 | UPPER_SNAKE_CASE | MAX_RETRIES |

### Include 顺序
1. 对应头文件（如 Foo.cpp 首行 `#include "Foo.h"`）
2. 本地项目头文件
3. Qt 头文件
4. 标准库头文件
> 头文件内优先用前置声明，.cpp 再 include 全部依赖

### 类型与容器
- Qt 相关优先用 Qt 容器（QString、QList、QVector），核心层可用 std 容器
- 明显场景可用 auto，否则显式类型
- 网络/序列化优先用定长整型（int32_t、qint32 等）

### 内存管理
- QObject 派生类务必传 parent，交由 Qt 对象树自动回收
- 其他对象用智能指针，禁止裸 new/delete

### 错误处理
- 早返回，避免嵌套
- 网络相关必须连接 errorOccurred/finished 信号，**绝不假定网络一定成功**
- Qt 事件循环下避免抛异常，优先返回 bool/std::optional

### 注释与文档
- 命名自解释为主，复杂逻辑/特殊原因务必注释“为什么”
- 禁止遗留死代码，直接删除

### UI/Qt 约定
- 静态布局优先 .ui 文件，动态用 setupUi(this)
- 跨层通信全部用信号/槽（LosRouter），UI 层禁止直接依赖核心服务
- 阻塞/耗时操作必须异步或线程池，主线程只负责 UI 响应

---

## 5. 命名 / 架构差异说明

本文件为所有 AI 代理通用说明。**Perseus 独有的命名、架构、单例/工厂等特殊约定，请查阅 [docs 架构文档](1_architecture/)**：

- 命名前缀为 `L_`（普通成员）/ `LOS_`（服务对象成员），**非 `m_`**
- 单例统一为 **Meyers Singleton**（`static T& instance()`），调用 `LosXxx::instance().method()`
- 详细模块职责见 [modules.md](1_architecture/modules.md)，信号机制见 [overview.md](1_architecture/overview.md)，
  设计模式见 [design-patterns.md](1_architecture/design-patterns.md)，插件开发见 [2_extensions/](2_extensions/)

---

## 6. 常见问题与陷阱

- Qt 6/Clang-18/Ninja/WSL2 环境要求严格，务必用 env.sh 一键安装依赖
- 所有构建/测试/格式化命令请严格按本文件和 docs/ 执行，勿自行假设
- 业务逻辑严禁进入 UI 层，所有跨层通信用 LosRouter 信号
- 插件开发、扩展协议等请查阅 docs/2_extensions/

---

## 7. 维护与贡献

如需补充/修正本说明，请优先链接已有文档，避免重复。遇到复杂场景建议拆分 skill 文件或专用 agent 说明。

