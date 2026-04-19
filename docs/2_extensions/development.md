# 插件开发指南

> 本文档介绍如何开发 Perseus 插件。

---

## 1. 开发环境准备

### 1.1 环境要求

| 要求 | 说明 |
|------|------|
| Node.js | 插件运行时 (部分插件) |
| Perseus | 最新版 IDE |
| Git | 版本控制 |

### 1.2 创建插件项目

```bash
# 创建插件目录
mkdir -p plugins/my-plugin
cd plugins/my-plugin

# 创建 package.json
cat > package.json <<EOF
{
    "name": "my-plugin",
    "version": "1.0.0",
    "description": "我的第一个插件",
    "main": "index.js",
    "author": "你的名字",
    "contributes": {
        "commands": [
            {
                "command": "my-plugin.hello",
                "title": "Hello 命令"
            }
        ]
    }
}
EOF
```

---

## 2. 插件结构

### 2.1 目录结构

```
my-plugin/
├── package.json      # 插件清单
├── index.js       # 入口文件
├── README.md     # 说明文档
└── resources/   # 资源文件
    └── icon.png
```

### 2.2 package.json 字段

| 字段 | 必填 | 说明 |
|-------|-----|------|
| `name` | 是 | 插件唯一 ID |
| `version` | 是 | 版本号 |
| `description` | 是 | 插件描述 |
| `main` | 是 | 入口文件 |
| `author` | 否 | 作者 |
| `contributes` | 否 | 扩展内容 |

---

## 3. 命令扩展

### 3.1 注册命令

在 `contributes.commands` 中注册：

```json
{
    "contributes": {
        "commands": [
            {
                "command": "my-plugin.hello",
                "title": "Hello 命令",
                "category": "My Plugin"
            }
        ]
    }
}
```

### 3.2 实现命令

在 `index.js` 中实现：

```javascript
// index.js
module.exports = {
    activate: function() {
        // 注册命令处理函数
        perseus.commands.registerCommand('my-plugin.hello', function() {
            perseus.window.showInformationMessage('Hello from my-plugin!');
        });
    },
    
    deactivate: function() {
        // 清理资源
    }
};
```

---

## 4. 快捷键扩展

### 4.1 注册快捷键

```json
{
    "contributes": {
        "commands": [
            {
                "command": "my-plugin.hello",
                "title": "Hello 命令"
            }
        ],
        "keybindings": [
            {
                "command": "my-plugin.hello",
                "key": "ctrl+shift+h"
            }
        ]
    }
}
```

---

## 5. API 参考

### 5.1 可用 API

| API | 说明 |
|-----|------|
| `perseus.commands.registerCommand()` | 注册命令 |
| `perseus.commands.executeCommand()` | 执行命令 |
| `perseus.window.showInformationMessage()` | 显示信息 |
| `perseus.window.showWarningMessage()` | 显示警告 |
| `perseus.window.showErrorMessage()` | 显示错误 |
| `perseus.workspace.openFile()` | 打开文件 |
| `perseus.workspace.showFile()` | 显示文件 |

---

## 6. 调试插件

### 6.1 开启调试模式

1. 打开命令面板 (`Ctrl+Shift+P`)
2. 输入 `Developer: Reload Plugins`
3. 重载插件

### 6.2 查看日志

日志输出到主程序日志中。

---

## 7. 发布插件

### 7.1 打包插件

```bash
# 打包为 zip
zip -r my-plugin.zip my-plugin/
```

### 7.2 提交到插件市场

请参考插件分发文档。

---

## 8. 示例插件

### 8.1 Hello World 插件

```javascript
// 完整示例
module.exports = {
    activate: function() {
        // 注册命令
        perseus.commands.registerCommand('my-plugin.hello', () => {
            perseus.window.showInformationMessage('Hello World!');
        });
        
        // 注册快捷键
        perseus.commands.registerKeybinding('my-plugin.hello', 'ctrl+shift+h');
    },
    
    deactivate: function() {
        // 清理
    }
};
```