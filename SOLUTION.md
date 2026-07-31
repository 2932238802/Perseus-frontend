# 问题解决方案

---

## 1. Ctrl+左键鼠标指针行为

### 问题分析
- 位置: `src/view/LosEditorUi/LosEditorUi.cpp:832-856` (`updateHoverUnderline`) 及 `:1340-1348` (`onControlKeyPressed`)
- 当前: 按住 Ctrl 时如果鼠标悬停在任意单词上就显示 `PointingHandCursor`，与 VSCode 行为不一致
- VSCode 行为: 仅当鼠标悬停在**可导航符号**（LSP 能解析的定义/引用）上时才显示链接手势，其余区域保持 `IBeamCursor`

### 解决思路

**方案 A（推荐，轻量）**: 增强 `updateHoverUnderline()` 的过滤条件

当前仅检查 `isLetterOrNumber() || '_'`，过于宽泛。改进:

1. 在 `LosEditorUi.cpp:837-842` 增加更严格的 token 类型过滤，忽略:
   - 语言关键字 (if/else/for/while/class/struct/return 等)
   - 注释和字符串字面量中的内容
   - 纯数字常量
2. 在 `onControlKeyPressed()` 中不直接调 `updateHoverUnderline`，改为仅置标志位；由 `mouseMoveEvent` 驱动光标变化
3. 增加 `leaveEvent` 处理：鼠标离开编辑器视口时立即恢复 `IBeamCursor`

**方案 B（长期）**: 接入 LSP hover 结果判断可导航性

1. 按下 Ctrl 时向 LSP 请求 hover 信息（已有 `_cmd_lsp_request_hover` 信号，`LosRouter.h:39`）
2. 仅当 hover 返回非空结果时才切换 `PointingHandCursor`
3. 需要关注延迟体验，可做 150ms 防抖

**推荐模式**: 无需额外设计模式，利用现有 signal/slot（Observer 模式）

---

## 2. 设置 UI 左侧栏目被遮挡 / 窗口从左上角弹出

### 问题分析

**遮挡问题**:
- `src/view/LosSettingsUi/LosSettingsUi.ui:33` — `category_list` 是 `QListWidget`，minWidth=160, maxWidth=320
- 随着主题字体放大，项目文字 "Build & Run", "Install Tools" 等可能超出容器宽度
- 没有设置 `QListWidget::setWordWrap(false)` 或水平滚动条

**窗口弹出位置**:
- `src/Perseus.cpp:508`: `LOS_setting->exec()` 作为 `QDialog` 模态调用
- 若父窗口尚未完成首次 show/布局，`exec()` 无法正确居中，回退到左上角 (0,0)
- 可能原因: `Perseus` 构造函数中创建 `LOS_setting`（line 487）早于 `Perseus::show()`

### 解决思路

**遮挡修复**:

1. 在 `LosSettingsUi::initStyle()` 中设置:
   ```cpp
   ui->category_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   ui->category_list->setTextElideMode(Qt::ElideNone);
   ```
2. 将 `category_list` 的 `minimumWidth` 从 160px 增大到 180px（`.ui` 文件）
3. 若文字仍溢出，给每个 item 设置 `setSizeHint` 使高度自适应，并启用 `setWordWrap(true)`

**窗口位置修复**:

1. 在 `LosSettingsUi` 构造函数末尾，延迟到下一个事件循环居中:
   ```cpp
   QMetaObject::invokeMethod(this, [this]() {
       if (parentWidget())
           move(parentWidget()->geometry().center() - rect().center());
   }, Qt::QueuedConnection);
   ```
   或
2. 在 `Perseus.cpp:508` 处改为:
   ```cpp
   connect(&router, &LosCore::LosRouter::_cmd_settingBtnClick, this, [this]() {
       LOS_setting->resize(LOS_setting->sizeHint());
       LOS_setting->move(frameGeometry().center() - LOS_setting->rect().center());
       LOS_setting->exec();
   });
   ```

---

## 3. Format 栏目默认显示 FORMAT_STYLE

### 问题分析
- `src/view/LosSettingsUi/LosSettingsUi.cpp:376-391` — `onFormatReload()` 在 `.clang-format` 文件不存在时显示占位提示文
- 应改为显示 `LLVM_formatStyle.h` 中的 `FORMAT_STYLE` 常量作为默认模板
- `FORMAT_STYLE` 定义在 `src/common/constants/ConstantsStr/LLVM_formatStyle.h:13`

### 解决思路

1. 在 `LosSettingsUi.cpp` 顶部 include:
   ```cpp
   #include "common/constants/ConstantsStr/LLVM_formatStyle.h"
   ```
2. 修改 `onFormatReload()` 的 else 分支（line 385-387）:
   ```cpp
   // 原来:
   ui->edit_clang_format->setPlainText(QStringLiteral("# .clang-format not found in project root.\n"
                                                        "# Type your config here and click Save to create it.\n"));
   // 改为:
   ui->edit_clang_format->setPlainText(QString::fromUtf8(LosCommon::FORMAT_STYLE));
   ```
3. 用户可以直接在默认内容基础上修改后保存；`Reload` 按钮仍从磁盘读取

---

## 4. Install Tools sudo 密码输入弹窗

### 问题分析
- `src/view/LosSettingsUi/LosSettingsUi.cpp:249` — `onCMakeInstallBtnClicked()` 为空，其他工具安装按钮尚未连接
- `src/core/LosRunner/LosScriptRunner/LosScriptRunner.cpp:163-178` — `onAutoInstallTool()` 通过 `LosAbstractRunner` 基类启动 QProcess 运行安装脚本
- 安装脚本（如 `apt install`）需要 sudo 权限，当前无密码传递机制
- 安装完成后需要通知 UI 刷新按钮状态

### 解决思路

**1. 新增 Sudo 密码对话框** (`src/view/LosSudoPasswordUi/`)

```
文件:
  LosSudoPasswordUi.h     — 继承 QDialog, QLineEdit(EchoMode::Password) + 确认/取消按钮
  LosSudoPasswordUi.cpp   — 密码以 QString 返回, 不可持久化
  LosSudoPasswordUi.ui    — 布局: 提示标签 + 密码输入框 + 按钮
```

**2. 修改安装流程**

- 点击 Install → 先弹出 `LosSudoPasswordDialog` → 用户输入密码点确认
- 将密码临时传递给 `LosScriptRunner::onAutoInstallTool()` 增加密码参数
- `LosScriptRunner` 通过 QProcess 写入 `echo <password> | sudo -S <install_cmd>` 或设置环境变量 `SUDO_ASKPASS`
- **安全设计**: 密码不落盘，仅保存在当前 QProcess 会话内存中；安装完成后立即置空

**3. 安装状态通知 (LosRouter 新增信号)**

```cpp
// LosRouter.h 新增:
void _cmd_installToolStarted(LosCommon::LosToolChain_Constants::LosTool tool);
void _cmd_installToolFinished(LosCommon::LosToolChain_Constants::LosTool tool, bool success);
void _cmd_installToolOutput(LosCommon::LosToolChain_Constants::LosTool tool, const QString &output);
```

- `LosScriptRunner` 监听 QProcess 的 `readyRead/finished` 信号，转发到 Router
- `LosSettingsUi` 连接 `_cmd_installToolFinished` → 成功则调 `setInstalled(tool)` 更新按钮状态
- 安装输出可实时显示在终端面板

**4. 连接所有 Install 按钮**

`LosSettingsUi.cpp:initConnect()` 中为所有 `btn_install_*` 连接统一的 slot:
```cpp
connect(ui->btn_install_cmake, &QPushButton::clicked, this, [this]() { onInstallBtnClicked(LosTool::CMAKE); });
connect(ui->btn_install_ninja,  &QPushButton::clicked, this, [this]() { onInstallBtnClicked(LosTool::NINJA); });
// ... 其他按钮类同
```

**设计模式推荐**:
- **PImpl**: 将安装状态机、密码管理、输出缓冲等私有实现封装在 `LosScriptRunnerPrivate` 中，减少头文件暴露
- **State Machine**: 安装流程状态: `Idle → PasswordRequested → Installing → Success/Failed`，避免 `L_installing` / `L_passReady` 等多个 bool 冲突
- **Command**: 每个工具安装任务封装为 `LosInstallCommand` 对象，携带 tool、script、状态、错误信息，支持取消和重试
- **Strategy**: `LosInstallBackend` 抽象基类，`LosAptBackend` / `LosPipBackend` / `LosCargoBackend` 分别实现不同包管理器的安装策略
- **Factory**: `LosInstallFactory::create(tool)` 根据工具类型返回对应的 `LosInstallCommand`

---

## 5. Terminal Settings 页面配置

### 问题分析
- `src/view/LosSettingsUi/LosSettingsUi.ui:226-271` — `page_terminal` 仅含一个占位标签
- `src/core/LosTerminal/LosTerminal.h:14-36` — 当前仅支持进程管理，无可配置项

### 解决方案

**可配置项设计**:

| 配置项 | 说明 | 实现方式 |
|--------|------|----------|
| Shell 路径 | 默认 `/bin/bash`，可选 zsh/fish | QComboBox 或 QLineEdit + 浏览 |
| 启动目录 | 默认项目根目录，可选 Home | QLineEdit + 浏览按钮 |
| 字体 | 终端等宽字体选择 | QFontComboBox (filter monospace) |
| 字号 | 12~24 | QSpinBox |
| 颜色方案 | 内置 4~6 套终端主题 | QComboBox 预览 |
| 光标样式 | Block / Underline / Beam | QComboBox |
| 光标闪烁 | 开/关 | QCheckBox |
| 回滚行数 | 1000~50000 | QSpinBox |
| 环境变量 | 附加 PATH/PYTHONPATH 等 | QTableWidget (key-value) |
| 终端历史 | 跨会话保留命令历史 | QCheckBox |

**持久化**: 使用 `QSettings`，key 为 `Terminal/<key>`

**实现文件**:
- `src/view/LosSettingsUi/LosSettingsUi.ui` — 扩展 `page_terminal` 的 UI 控件
- `src/view/LosSettingsUi/LosSettingsUi.cpp` — 增加 `initTerminalPage()` / `onTerminalSettingChanged()`
- `src/core/LosTerminal/LosTerminalConfig.h` — 终端配置数据结构
- `LosTerminal` 启动时从 `QSettings` 读取并应用配置

**设计模式推荐**:
- **Strategy**: `LosTerminalBackend` 接口，`LosBashBackend` / `LosZshBackend` 分别处理不同 shell 的启动参数和转义序列差异
- **PImpl**: `LosTerminal` 的配置加载/应用逻辑封装在 `LosTerminalPrivate` 中

---

## 设计模式总结

| 模式 | 应用于 | 理由 |
|------|--------|------|
| **PImpl** | `LosScriptRunnerPrivate` (安装密码/状态机), `LosTerminalPrivate` (配置) | 减少 QProcess/QSettings 头文件扩散，隐藏状态机内部 flag |
| **State Machine** | 安装流程: `Idle → Password → Installing → Done/Failed` | 替代 `L_installing` / `L_hasPassword` 等多 bool 互斥问题 |
| **Command** | `LosInstallCommand` (tool + script + state + error) | 封装单个安装任务，支持重试、取消、输出流式推送 |
| **Strategy** | `LosInstallBackend` (apt/pip/cargo), `LosTerminalBackend` (bash/zsh) | 不同后端实现统一接口，运行时根据平台/tool 类型选择 |
| **Factory** | `LosInstallFactory::create(LosTool)` → `LosInstallCommand` | 隔离创建逻辑，方便后续扩展新工具 |
| **Observer** | Qt signal/slot 天然支持，LosRouter 信号总线 | 安装状态变化通知 UI 刷新、终端输出实时回显 |
| **Plugin** | 预留接口，本阶段不实现 | 未来可支持第三方工具安装器或终端模拟器后端 |

---

## 实施优先级建议

| 优先级 | 问题 | 预计工时 |
|--------|------|----------|
| P0 (立即可修) | #3 Format 默认内容 — 改动 1 行 | 5 分钟 |
| P0 (立即可修) | #2 窗口弹出位置 — 改动 3 行 | 10 分钟 |
| P1 (本周) | #1 Ctrl+Click 光标 — 方案 A | 1~2 小时 |
| P1 (本周) | #2 左侧栏目遮挡 — UI 调参 | 30 分钟 |
| P2 (下周) | #4 Install Tools 密码弹窗 — 全套流程 | 2~3 天 |
| P2 (下周) | #5 Terminal Settings — UI + 配置读存 | 1~2 天 |
