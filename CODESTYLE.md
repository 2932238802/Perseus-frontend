# Perseus 代码风格 (CODESTYLE)

> 本文件供 opencode 自动加载，规定「怎么写每一行代码」。
> 构建/架构/命令见 AGENTS.md 与 CLAUDE.md，此处不重复。

## 文件头
- 每个 .h/.cpp 首行版权注释：`// Copyright (c) 2026 LosAngelous (shengjie.lin)`
- 头文件用 `#pragma once`（不用 include guard 宏）
- 头文件和版权注释 之间不要有空行，所以分别是第二行和第一行

## 命名（严格遵守，勿用 m_）
| 实体 | 规范 | 示例 |
|------|------|------|
| 类 / 结构体 | PascalCase | `LosRunManager` |
| 方法 / 函数 | camelCase | `initConnect()` |
| 普通成员变量 | `L_` 前缀 | `L_timer`、`L_dirty` |
| 持有的服务对象成员 | `LOS_` 前缀 | `LOS_runMgr`、`LOS_tabUi` |
| 槽函数 | `on`/`On` 前缀 | `onLog()`、`OnFileLoaded()` |
| 常量 / 宏 | UPPER_SNAKE_CASE | `MAX_RETRIES` |
| 命名空间 | `Los` 前缀 | `LosCore`、`LosView`、`LosModel`、`LosCommon` |

## 格式（.clang-format 已固化，改完必跑）
- LLVM 基础风格，**列宽 150**，4 空格缩进，禁止 Tab
- **Allman 大括号**（左花括号独占一行）
- 指针右对齐：`Type *ptr`
- include 字母序自动排序
- 成员变量声明时**等号对齐**（AlignConsecutiveAssignments）
- 成员变量在声明处就地初始化（`= nullptr` / `= false` / `= {}`）
- 提交前必须：`clang-format -i path/to/file.cpp path/to/file.h`

## include 顺序（.h 与 .cpp 分组，组间空行）
1. 对应头文件（Foo.cpp 首行 `#include "Foo.h"`）
2. 本地项目头文件（`"common/..."` `"core/..."` `"models/..."` `"view/..."`）
3. Qt 头文件（`<QString>`）
4. 标准库头文件（`<memory>`）
- .h 里优先前置声明，完整依赖只在 .cpp 里 include

## 类结构约定（按 Perseus.h 的分区写法）
- 用带注释的访问区分段：`protected: // override`、`private: // tool`、`public slots: // xhc`
- 在类声明上方用块注释（`/* ... */`）描述关键方法职责与 TODO
- QObject 派生类首行 `Q_OBJECT`，构造函数 `explicit`，parent 默认 `nullptr`

## 内存管理
- QObject 派生类：构造时传 `this` 作 parent，交给 Qt 对象树回收
- 非 QObject：用 `std::unique_ptr` / `std::shared_ptr`
- 禁止裸 `new`/`delete`（除非立刻转交 parent 或智能指针）

## 错误处理
- 早返回，避免嵌套；返回前用 `ERR()` / `qWarning()` 记录
- 优先返回 `bool` / `std::optional`，不用异常（与 Qt 事件循环冲突）
- 网络调用绝不假定成功，必须连 `errorOccurred` / `finished` 信号

## 跨层通信
- 一律通过 `LosRouter` 信号，View 层禁止直接调用 core 服务
- 全局状态走 `LosState`，日志用 `INF/WAR/ERR/SUC` 宏

## 注释
- 命名自解释优先；复杂逻辑注释「为什么」而非「做什么」
- 禁止遗留死代码，直接删除
- 尽量不要写注释
