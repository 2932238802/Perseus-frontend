# libgit2 集成指南（Submodule 方式）

> 本文记录 Perseus 如何以 **git submodule + CMake add_subdirectory** 的方式集成 libgit2，
> 以及 clone 项目后的初始化、日常维护和踩坑记录。
>
> **本项目不使用 `apt install` 或 `vcpkg` 安装 libgit2**，原因见下文。

---

## 1. 为什么选 submodule，而不是 apt / vcpkg / FetchContent

| 方式 | 问题 | 是否采用 |
|------|------|----------|
| `apt install libgit2-dev` | 版本由发行版锁定（Ubuntu 24.04 给的偏旧），团队成员/CI 版本不一致，无法精确控制特性开关 | ❌ |
| `vcpkg` | 引入额外的包管理器依赖，构建链路变长，与现有纯 CMake 流程不统一 | ❌ |
| `FetchContent` | 每次配置可能触发网络下载，源码不进版本库，离线/审计不友好 | ❌ |
| **git submodule + add_subdirectory** | 版本随提交锁定（commit 级精确）、源码物理隔离在 `third_party/`、纯 CMake 集成、可离线、可审计 | ✅ **采用** |

**核心理由**：submodule 把 libgit2 的**精确 commit** 记录进本仓库，任何人 clone 后拿到的都是同一份源码，编译产物可复现；同时所有源码集中在 `third_party/`，与业务代码 `src/` 物理隔离。

---

## 2. 当前集成现状（事实速查）

| 项 | 值 |
|----|----|
| submodule 路径 | `third_party/libgit2` |
| 上游 URL | `https://github.com/libgit2/libgit2.git` |
| 锁定版本 | **v1.9.4** |
| 链接 target 名 | **`libgit2package`**（⚠️ 不是 `libgit2`） |
| HTTPS 后端 | OpenSSL 3.0.13（系统 `libssl-dev`） |
| 链接方式 | 静态库（`BUILD_SHARED_LIBS OFF`） |
| SSH | 当前关闭（`USE_SSH OFF`），留作第二阶段 |

---

## 3. 如何添加 submodule（首次集成，已完成，仅作记录）

> 这一步**只需做一次**，已经做过了。这里记录是为了让你知道当初是怎么加进来的。

```bash
# 在项目根目录执行
git submodule add https://github.com/libgit2/libgit2.git third_party/libgit2

# 进入 submodule，切到稳定 tag（而不是用上游 master 的浮动状态）
cd third_party/libgit2
git checkout v1.9.4
cd ../..

# 回到项目根，提交：此时会记录 .gitmodules + libgit2 的精确 commit
git add .gitmodules third_party/libgit2
git commit -m "chore: add libgit2 v1.9.4 as submodule"
```

执行后会生成 `.gitmodules` 文件：

```ini
[submodule "third_party/libgit2"]
	path = third_party/libgit2
	url = https://github.com/libgit2/libgit2.git
```

> ⚠️ **submodule 锁的是 commit，不是 tag**。`.gitmodules` 里只有 path 和 url，
> 具体停在哪个 commit 由父仓库的 "gitlink" 记录（`git submodule status` 可查）。
> 所以切到 `v1.9.4` 后必须 `git add` + `commit`，才能把这个 commit 锁进父仓库。

---

## 4. Clone 项目后如何初始化（⭐ 新人/换机器必读）

普通 `git clone` **不会**自动拉取 submodule 的内容，`third_party/libgit2` 会是个空目录。
两种方式拉取：

**方式一：clone 时一并拉取（推荐）**
```bash
git clone --recurse-submodules <perseus 仓库地址>
```

**方式二：已经 clone 了，再补拉**
```bash
git submodule update --init --recursive
```

> `--recursive` 是因为 libgit2 自己可能还嵌套了 submodule，要一层层拉全。
> 如果 `third_party/libgit2` 是空的、CMake 报 `add_subdirectory ... does not contain a CMakeLists.txt`，
> 八成就是忘了这一步。

---

## 5. CMake 集成详解

集成代码在根 `CMakeLists.txt` 中，逐段解释：

```cmake
add_library(PerseusCore STATIC ${CORE_SOURCES})

# ---------- libgit2 ----------
# 覆盖 libgit2 的构建选项。必须用 CACHE + FORCE，否则 add_subdirectory 时
# libgit2 自己的 option() 会用它的默认值覆盖我们的设置。
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)   # 静态链接，不产出 .so
set(BUILD_TESTS       OFF CACHE BOOL "" FORCE)   # 不编译 libgit2 的测试
set(BUILD_CLI         OFF CACHE BOOL "" FORCE)   # 不编译 git2 命令行工具
set(BUILD_EXAMPLES    OFF CACHE BOOL "" FORCE)   # 不编译示例

set(USE_SSH   OFF CACHE STRING "" FORCE)         # 第一阶段不做 SSH（注意是 STRING 类型）
set(USE_HTTPS ON  CACHE STRING "" FORCE)         # 走 HTTPS，后端用系统 OpenSSL
set(USE_NSEC  OFF CACHE BOOL "" FORCE)           # 关闭纳秒时间戳探测（见踩坑 #1）

# 把 libgit2 源码目录纳入构建
add_subdirectory(third_party/libgit2)

# 链接：注意 target 名是 libgit2package，不是 libgit2（见踩坑 #2）
target_link_libraries(PerseusCore PUBLIC libgit2package)

# 手动暴露头文件目录（见踩坑 #3）
target_include_directories(PerseusCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libgit2/include
)
```

### 关键点说明

- **为什么用 `CACHE ... FORCE`**：libgit2 内部用 `option(BUILD_TESTS ...)` 定义这些开关。
  `option` 本质是 cache 变量，如果你只用 `set(BUILD_TESTS OFF)`（普通变量），
  `add_subdirectory` 进去后 libgit2 的 `option()` 仍会把 cache 值设回默认。
  必须 `set(... CACHE BOOL "" FORCE)` 强制覆盖 cache，才能压住它。

- **`USE_SSH` / `USE_HTTPS` 是 STRING 不是 BOOL**：libgit2 这两个选项允许填后端名（如 `"OpenSSL"`），
  所以类型是 STRING。用 `ON`/`OFF` 也能被识别，但类型要写对 `CACHE STRING`。

- **`PUBLIC` 链接 + `PUBLIC` include 的意义**：PerseusCore 用 `PUBLIC` 链接 libgit2package，
  意味着**任何链接 PerseusCore 的 target（如 Perseus 主程序、test）都自动继承** libgit2 的链接和头文件路径，
  不用在每个地方重复配置。

---

## 6. 日常维护

### 升级 libgit2 版本
```bash
cd third_party/libgit2
git fetch --tags
git checkout v1.9.5          # 切到目标 tag
cd ../..
git add third_party/libgit2  # 记录新 commit 到父仓库
git commit -m "chore: bump libgit2 to v1.9.5"
```
升级后**务必重新 configure + 全量编译**验证，特性开关或 API 可能有变化。

### 拉取队友更新了 submodule 指向后同步
```bash
git pull
git submodule update --init --recursive   # 同步到队友锁定的新 commit
```

---

## 7. 踩坑记录（重要，省下重复踩坑的时间）

### 坑 #1：`GIT_USE_NSEC defined but unknown struct stat nanosecond type`
- **现象**：编译 libgit2 时报错，找不到 struct stat 的纳秒字段类型。
- **原因**：libgit2 用 C90 模式探测 `struct stat` 的纳秒精度字段，
  在 clang-18 + 当前环境下探测失败，但 `USE_NSEC` 又被默认打开，导致引用了不存在的字段。
- **解决**：`set(USE_NSEC OFF CACHE BOOL "" FORCE)` 显式关闭纳秒时间戳支持。
  对 IDE 的 git 追踪功能而言，秒级精度足够，关掉无影响。

### 坑 #2：链接 target 名是 `libgit2package`，不是 `libgit2`
- **现象**：`target_link_libraries(... libgit2)` 报找不到 target。
- **原因**：libgit2 的 CMake 里，`libgit2` 是一个 **OBJECT 库**（中间产物），
  真正可被外部链接的打包 target 叫 **`libgit2package`**。
- **解决**：`target_link_libraries(PerseusCore PUBLIC libgit2package)`。

### 坑 #3：`#include <git2.h>` 找不到头文件
- **现象**：能链接但编译时找不到 `git2.h`。
- **原因**：libgit2 用 `INSTALL_INTERFACE` 暴露头文件目录，
  这是给"安装后"用的；`add_subdirectory` 直接用源码树时，这个 include 路径**不会自动传递**给依赖方。
- **解决**：手动 `target_include_directories(PerseusCore PUBLIC .../third_party/libgit2/include)`。

### 坑 #4：GLOB 不会自动发现新文件
- 与 libgit2 无关，但相关：本项目用 `file(GLOB_RECURSE)` 收集源码，
  **新增 .cpp/.h 后必须重新 `cmake -S . -B build` 配置**，否则新文件不会被编译。

---

## 8. 验证集成是否成功

最小验证：能 `#include <git2.h>` 并调用 `git_libgit2_init()` 且编译链接通过，即说明集成 OK。
本项目中由 `LosCore::LosGitManager` 单例在构造时调用 `git_libgit2_init()`、析构时 `git_libgit2_shutdown()`。

```bash
# 重新配置（新增文件后必做）+ 编译核心库
cmake -S . -B build
cmake --build build --target PerseusCore
# 出现 "Built target PerseusCore" 即集成成功
```

---

## 相关文档
- 架构总览：`docs/1_architecture/`
- 环境搭建：`docs/3_development/setup.md`
- Git 工作流：`docs/3_development/git-workflow.md`
