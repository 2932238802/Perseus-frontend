# 主题系统

> 本文档介绍 Perseus 的主题系统和样式配置。

---

## 1. 主题概览

Perseus 采用 Qt Style Sheets (QSS) 实现主题系统，支持自定义样式。

---

## 2. 样式文件结构

### 2.1 样式文件位置

```
src/view/style/
├── perseus_style.h           # 主样式
├── LosToolMissUI_style.h  # 工具缺失提示样式
├── LosIssuesUi_style.h   # 问题面板样式
├── LosCommandUi_style.h  # 命令面板样式
└── LosCompleterUi_style.h # 补全面板样式
```

### 2.2 主样式内容

```cpp
// perseus_style.h
static const char* g_perseus_style = R"(
QMainWindow {
    background-color: #1e1e1e;
}

QPlainTextEdit {
    background-color: #1e1e1e;
    color: #d4d4d4;
    font-family: 'JetBrains Mono', 'Consolas', monospace;
    font-size: 14px;
}
)";
```

---

## 3. 颜色系统

### 3.1 基础颜色

| 用途 | 颜色值 |
|------|--------|
| 背景 (深色) | `#1e1e1e` |
| 背景 (浅色) | `#252526` |
| 边框 | `#3c3c3c` |
| 文字 (主色) | `#d4d4d4` |
| 文字 (次色) | `#858585` |

### 3.2 语法高亮颜色

| 类型 | 颜色值 |
|------|--------|
| 关键词 | `#E17899` |
| 预处理指令 | `#B97EE6` |
| 类/类型 | `#F5B83D` |
| 函数 | `#2DCCCF` |
| 字符串 | `#42E6A4` |
| 注释 | `#5B7888` |
| 数字 | `#79b8ff` |
| 运算符 | `#ff7b72` |

---

## 4. 字体配置

### 4.1 代码字体

```
Font: JetBrains Mono, Consolas, Courier New
Size: 14px
Line Height: 1.5
```

### 4.2 UI 字体

```
Font: Segoe UI, Microsoft YaHei
Size: 13px
```

---

## 5. 主题切换

### 5.1 加载主题

```cpp
void Perseus::initStyle()
{
    QFile styleFile(":/style/perseus.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    setStyleSheet(styleSheet);
}
```

---

## 6. 组件样式示例

### 6.1 按钮样式

```css
QPushButton {
    background-color: #0e639c;
    color: #ffffff;
    border: none;
    padding: 6px 12px;
    border-radius: 2px;
}

QPushButton:hover {
    background-color: #1177bb;
}
```

### 6.2 滚动条样式

```css
QScrollBar:vertical {
    background: #1e1e1e;
    width: 14px;
}

QScrollBar::handle:vertical {
    background: #424242;
    border-radius: 7px;
}
```

---

## 7. 自定义主题

用户可以通过修改 QSS 文件自定义主题：

```bash
# 自定义主题文件位置
~/.perseus/themes/dark.qss
```

---

## 8. 主题变量

```css
:root {
    --bg-primary: #1e1e1e;
    --bg-secondary: #252526;
    --bg-tertiary: #2d2d2d;
    --text-primary: #d4d4d4;
    --text-secondary: #858585;
    --accent: #0e639c;
    --accent-hover: #1177bb;
    --border: #3c3c3c;
    --error: #f14c4c;
    --warning: #cca700;
    --success: #89d185;
}
```