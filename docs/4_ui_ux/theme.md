# 主题系统

> 本文档介绍 Perseus 的主题系统、样式组织与切换机制。

---

## 1. 主题概览

Perseus 采用 **Qt Style Sheets (QSS) + Token 注入** 的方式实现可切换的多主题系统，
由 `LosCore::LosThemeManager`（单例）统一管理。

- **默认主题**：`dracula`
- **支持运行时切换**：切换后广播 `_cmd_themeChanged(themeName)`，各 UI 重新应用样式
- **机制**：QSS 模板中使用 token 占位（如颜色变量），`LosThemeManager` 按当前主题的
  token 表替换占位，生成最终 QSS 并 `setStyleSheet` 应用

---

## 2. 样式文件组织

### 2.1 样式文件位置

```
src/view/style/
├── perseus_style.qss          # 主样式模板（QSS，~22KB，含 token 占位）
├── perseus_style.h            # 主样式头（加载/包装）
│
├── LosAgent_style.h           # AI 助手面板样式
├── LosAgentKeyUi_style.h      # AI Key 配置样式
├── LosAuthUi_style.h          # 登录/注册对话框样式
├── LosMDPreview_style.h       # Markdown 预览样式
├── LosIssuesUi_style.h        # 问题面板样式
├── LosCommandUi_style.h       # 命令面板样式
├── LosCommandArgsUi_style.h   # 命令参数输入样式
├── LosCompleterUi_style.h     # 补全弹窗样式
└── LosToolMissUI_style.h      # 工具链缺失提示样式
```

> 主样式以 `.qss` 文件存放（经资源系统 `:/style/...` 加载）；各组件的局部样式以 C++ 头文件中的
> 字符串常量形式提供，便于按主题动态拼接。

---

## 3. LosThemeManager API

```cpp
class LosThemeManager : public QObject   // 单例
{
    static LosThemeManager& instance();

    QStringList availableThemes() const;            // 可用主题名列表
    QString currentTheme() const;                   // 当前主题（默认 "dracula"）
    QString currentDisplayName() const;             // 当前主题显示名
    bool setTheme(const QString &name, bool persist = true); // 切换并可持久化
    QJsonObject themeJson(const QString &name) const;        // 主题定义(JSON)

    QString buildMainQss(const QString &name) const;             // 生成主 QSS
    QString buildExtraQss(const QString &tmpl, const QString &name) const; // 生成组件 QSS
    QHash<QString, QString> uiTokens(const QString &name) const; // token -> 颜色
};
```

### 切换主题

```cpp
// 切换到指定主题（会持久化并广播 _cmd_themeChanged）
LosCore::LosThemeManager::instance().setTheme("dracula");

// 监听主题变化，重新应用局部样式
connect(&LosCore::LosRouter::instance(),
        &LosCore::LosRouter::_cmd_themeChanged,
        this, [this](const QString &name){
            this->applyTheme(name);   // 各 UI 自行实现
        });
```

### 应用主样式

```cpp
QString qss = LosCore::LosThemeManager::instance().buildMainQss(
                  LosCore::LosThemeManager::instance().currentTheme());
qApp->setStyleSheet(qss);
```

---

## 4. Token 注入机制

QSS 模板中用占位符表示颜色等可变项，`LosThemeManager` 用当前主题的 token 表替换：

```
模板 (perseus_style.qss)            token 表 (按主题)              最终 QSS
─────────────────────────         ─────────────────────         ─────────────────────
QMainWindow {                      bg-primary  -> #282a36         QMainWindow {
  background: @bg-primary;   ──▶   text        -> #f8f8f2   ──▶     background: #282a36;
  color: @text;                    accent      -> #bd93f9            color: #f8f8f2;
}                                                                 }
```

> 不同主题只需提供不同的 token 表，模板复用，保证全局风格一致。

---

## 5. Dracula 主题参考色

| 用途 | 颜色值 |
|------|--------|
| 背景（主） | `#282a36` |
| 背景（次 / 面板） | `#21222c` |
| 当前行 | `#44475a` |
| 前景文字 | `#f8f8f2` |
| 注释 / 次文字 | `#6272a4` |
| 强调（紫） | `#bd93f9` |
| 青 | `#8be9fd` |
| 绿 | `#50fa7b` |
| 黄 | `#f1fa8c` |
| 橙 | `#ffb86c` |
| 红 | `#ff5555` |
| 粉 | `#ff79c6` |

> 实际取值以各主题 JSON / token 表为准；语法高亮颜色由 `LosHighlighter` 结合主题 token 渲染。

---

## 6. 字体配置

### 6.1 代码字体

```
Font: JetBrains Mono, Consolas, Courier New
Size: 14px
```

### 6.2 UI 字体

```
Font: Segoe UI, Microsoft YaHei
Size: 13px
```

---

## 7. 组件样式示例

### 7.1 按钮

```css
QPushButton {
    background-color: @accent;
    color: @text-on-accent;
    border: none;
    padding: 6px 12px;
    border-radius: 4px;
}
QPushButton:hover {
    background-color: @accent-hover;
}
```

### 7.2 滚动条

```css
QScrollBar:vertical {
    background: @bg-primary;
    width: 12px;
}
QScrollBar::handle:vertical {
    background: @scrollbar-handle;
    border-radius: 6px;
}
```

---

## 8. 新增主题指引

1. 在主题定义（JSON / token 表）中新增一套 token（沿用现有 key，给出新配色）。
2. 在 `LosThemeManager` 的可用主题列表中登记该主题名与显示名。
3. 运行后通过设置面板或 `setTheme("your-theme")` 切换验证。
4. 若组件有专属样式头（`LosXxx_style.h`），确保其使用 token 占位而非硬编码颜色，
   以便随主题联动。
