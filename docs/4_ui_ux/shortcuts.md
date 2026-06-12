# 快捷键映射

> 本文档列出 Perseus 的快捷键映射。**全局快捷键的权威来源是
> `src/common/constants/ConstantsStr/ShortCut.h`**（`LosCommon::ShortCut` 命名空间），
> 由 `LosShortcutManager` 在 `Perseus::initShotcut()` 中注册。

---

## 1. 文件操作

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+O` | `FILE_OPEN` | 打开文件 / 文件夹 |
| `Ctrl+N` | `FILE_NEW` | 新建文件 |
| `Ctrl+S` | `FILE_SAVE` | 保存当前文件 |
| `Ctrl+Shift+S` | `FILE_SAVE_ALL` | 保存所有文件 |

---

## 2. 标签页

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+W` | `TAB_CLOSE` | 关闭当前标签页 |
| `Ctrl+Tab` | `TAB_NEXT` | 切换到下一个标签页 |

---

## 3. 编辑

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+Z` | `EDIT_UNDO` | 撤销 |
| `Ctrl+Shift+Z` | `EDIT_REDO` | 重做 |
| `Ctrl+Y` | `EDIT_REDO_ALT` | 重做（备用） |
| `Ctrl+X` | `LINE_DELETE` | 删除当前行 |
| `Ctrl+Shift+D` | `LINE_DUPLICATE` | 向下复制当前行 |
| `Alt+↑` | `LINE_MOVE_UP` | 当前行上移 |
| `Alt+↓` | `LINE_MOVE_DOWN` | 当前行下移 |
| `Ctrl+/` | `LINE_COMMENT` | 单行注释 / 取消注释 |
| `Shift+Alt+A` | `BLOCK_COMMENT` | 块注释 `/* ... */` |

---

## 4. 查找与跳转

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+F` | `SEARCH_FIND` | 页内查找 |
| `Ctrl+H` | `SEARCH_REPLACE` | 页内替换 |
| `Ctrl+G` | `GOTO_LINE` | 跳转到指定行 |
| `F12` | `GOTO_DEFINITION` | 跳转到定义 |
| `Ctrl+鼠标左键` | — | 跳转到定义（编辑器内 Hover + 点击） |

---

## 5. 构建与运行

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+F5` | `RUN_SINGLE_FILE` | 运行当前单文件 |
| `Ctrl+B` | `BUILD_PROJECT` | 编译整个工程（亦可 F7） |
| `F5` | `DEBUG_START` | 启动调试 |

---

## 6. 命令与格式化

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+Shift+P` | `COMMANDS` | 打开命令面板 |
| `Ctrl+K, Ctrl+O` | `CODE_FORMAT` | 格式化当前文档（clang-format） |

---

## 7. 视图与面板

| 快捷键 | 常量 | 功能 |
|--------|------|------|
| `Ctrl+Shift+E` | `TOGGLE_EXPLORER` | 显示 / 隐藏左侧文件树 |
| `Ctrl+J` | `TOGGLE_BOTTOM_PANEL` | 显示 / 隐藏底部面板 |
| `Ctrl+\`` | `FOCUS_TERMINAL` | 聚焦底部终端 |
| `Ctrl+Shift+/` | `TOGGLE_PREVIEW` | 切换标签页预览状态（Markdown 预览） |
| `Ctrl+>` | `FONT_ZOOM_IN` | 放大字体 / 全局缩放增大 |
| `Ctrl+<` | `FONT_ZOOM_OUT` | 缩小字体 / 全局缩放减小 |

---

## 8. 注册方式

快捷键在 `Perseus::initShotcut()` 中通过 `LosShortcutManager` 单例注册：

```cpp
LosCore::LosShortcutManager::instance().reg(
    LosCommon::ShortCut::GOTO_LINE,   // 快捷键字符串（来自 ShortCut.h）
    this,                             // 父窗口
    [this]() {                        // 回调
        // 显示跳转到行弹窗
    },
    "跳转到行"                        // 描述（可选）
);
```

> 新增快捷键时：**先在 `ShortCut.h` 中定义常量**，再在 `initShotcut()` 注册，
> 避免散落的硬编码字符串。

---

## 9. 快捷键冲突处理

当快捷键冲突时：
1. 优先使用当前焦点组件（如终端 / 编辑器）内部的快捷键；
2. 其余由全局 `LosShortcutManager` 注册的快捷键生效。
