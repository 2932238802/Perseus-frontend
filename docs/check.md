# 提交前自检清单（Checklist）

> 在提交 PR / 合并到 `main` 之前，逐项核对，避免 CI 失败与回归。

---

## 代码规范

- [ ] 已对改动文件执行 `clang-format -i`（基于根目录 `.clang-format`）
- [ ] 命名遵循约定：类 `PascalCase`，方法 `camelCase`，普通成员 `L_` 前缀，服务成员 `LOS_` 前缀，常量 `UPPER_SNAKE_CASE`
- [ ] 无遗留死代码 / 注释掉的废弃代码
- [ ] Include 顺序：对应头文件 → 本地头文件 → Qt 头文件 → 标准库

## 架构约定

- [ ] 业务逻辑未进入 UI 层；跨层 / 跨模块通信走 `LosRouter` 信号
- [ ] 新增单例使用 Meyers Singleton（`static T& instance()`）
- [ ] QObject 派生类正确传递 `parent`，无裸 `new/delete`
- [ ] 网络请求已连接错误信号，未假定一定成功

## 构建与运行

- [ ] 新增 `.cpp/.h` 后已重新 CMake 配置（`./re-build.sh`）
- [ ] Release 构建通过：`./re-build.sh`
- [ ] Debug 构建可运行：`./bbuild.sh`
- [ ] 关键功能本地手动验证通过

## 文档

- [ ] 如新增 / 修改模块或接口，已同步更新 `docs/` 相关文档
- [ ] 如新增快捷键，已在 `ShortCut.h` 定义并更新 `docs/4_ui_ux/shortcuts.md`

## 提交

- [ ] commit message 遵循 `[add]/[fix]/[fit]/[modify]/[del]/[docs]` 规范
- [ ] 一个 commit 只做一件事
- [ ] 已 rebase 到最新 `origin/main`
