# Git 协作流程

> 本文档规范 Perseus 项目的分支管理、提交与推送流程。

---

## 1. 分支模型

```
main                 ← 稳定主线，CI 自动构建
 └── dev-xxx         ← 功能开发分支，完成后合并回 main
```

| 分支 | 作用 | 示例 |
|------|------|------|
| `main` | 稳定版本，所有 CI/CD 基于此分支触发 | — |
| `dev-<功能名>` | 功能开发分支，从 main 创建 | `dev-cmake_lsp`、`dev-terminal`、`dev-plugin-ui` |
| `fix-<问题>` | 紧急修复分支 | `fix-editor-crash`、`fix-lsp-timeout` |

---

## 2. 开发流程

### 2.1 创建功能分支

```bash
git checkout main
git pull origin main
git checkout -b dev-你的功能名
git checkout -b dev-terminal-tabs
```

### 2.2 日常开发提交

```bash
# 查看当前修改状态
git status

# 添加指定文件（不要用 git add .，只加你改的文件）
git add src/view/LosTerminalTabsUi/LosTerminalTabsUi.cpp
git add src/view/LosTerminalTabsUi/LosTerminalTabsUi.h

# 提交
git commit -m "[add] 增加终端多标签切换功能"

# 推送到远程（首次加 -u）
git push -u origin dev-terminal-tabs
# 后续直接
git push
```

### 2.3 同步 main 最新代码（Rebase）

> 项目使用 **rebase** 而非 merge，保持提交历史线性、干净！！！

```bash
#  拉取远程 main 的最新提交
git fetch origin

# 将你的分支变基到最新的 main 上
git rebase origin/main

# 如果有冲突 ——
git add <冲突文件>
git rebase --continue
#    如果想放弃本次 rebase：
#    git rebase --abort

# 因为 rebase 改写了提交历史，需要 force push
#    --force-with-lease 比 --force 更安全
git push --force-with-lease
```

### 2.4 合并回 main

在 GitHub 上发起 **Pull Request**，Code Review 通过后合并。

或者本地操作：

```bash
git checkout main
git pull origin main
git rebase dev-...    
git push origin main

git branch -d dev-terminal-tabs
git push origin --delete dev-terminal-tabs
```

---

## 3. Commit 规范

项目使用 **`[类型] 描述`** 格式：

| 类型 | 含义 | 示例 |
|------|------|------|
| `[add]` | 新增功能 / 文件 | `[add] 增加插件界面` |
| `[fix]` | 修复 Bug | `[fix] 运行按钮 UI 显示问题` |
| `[fit]` | 完善 / 优化现有功能 | `[fit] 网络部分的完善` |
| `[modify]` | 重构 / 修改实现方式 | `[modify] 完善 pop 的常量表达` |
| `[del]` | 删除代码 / 文件 | `[del] 移除废弃的旧日志模块` |
| `[docs]` | 文档变更 | `[docs] 更新开发环境搭建指南` |

**书写原则：**

- 用中文简要描述**做了什么**
- 一个 commit 只做一件事，不要混合多个不相关的修改
- 如果改动较大，先在 commit message 第一行写概要，空一行后写详情：


---

## 4. 常用 Git 操作速查

### 撤销操作

```bash
# 撤销工作区修改 就是想要还原到 原来的分支
git checkout -- src/view/LosEditorUi/LosEditorUi.cpp

# 撤销 add（已 add 未 commit）
git reset HEAD src/view/LosEditorUi/LosEditorUi.cpp

# 撤销最近一次 commit（保留修改在工作区）
git reset --soft HEAD~1
```

### 同步远程更新

```bash
git fetch origin
git rebase origin/main
git push --force-with-lease
```

### 查看历史

```bash
git log --oneline -10
git log --oneline -- src/core/LosRouter/LosRouter.cpp
```

---

##  CI 说明

项目配置了 GitHub Actions（`.github/workflows/build.yml`），在以下情况自动触发 **Windows Release 构建**：

- 向 `main` 分支 push
- 向 `main` 分支发起 Pull Request
