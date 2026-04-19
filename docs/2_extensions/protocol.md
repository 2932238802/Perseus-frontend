# 插件协议

> 本文档介绍 Perseus 插件系统的接口协议和数据结构。

---

## 1. 插件系统架构

Perseus 插件系统采用**客户端-服务器+本地扩展**的架构模式：

```
+-------------------+      +------------------+      +------------------+
|   远程服务器      | ---> |   本地插件管理   | ---> |   插件运行时   |
|  (插件市场数据)  |      |  (加载/卸载)   |      |  (注入功能)   |
+-------------------+      +------------------+      +------------------+
```

---

## 2. 插件数据结构

### 2.1 插件信息结构

```cpp
namespace LosCommon::LosNet_Constants {

struct PluginInfo
{
    QString id;              // 插件唯一标识
    QString name;            // 插件名称
    QString version;          // 插件版本
    QString description;      // 插件描述
    QString author;          // 作者
    QString main;           // 入口文件
    QString repository;      // 仓库地址
    QString downloads;       // 下载次数
    QString icon;           // 图标 URL
    QStringList tags;        // 标签列表
};

}
```

### 2.2 插件清单文件

每个插件需要提供一个 `package.json` 文件：

```json
{
    "name": "my-plugin",
    "version": "1.0.0",
    "description": "我的插件描述",
    "main": "index.js",
    "author": "作者名",
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

## 3. 插件接口

### 3.1 命令接口

插件可以注册自定义命令：

```cpp
// 注册命令
void registerCommand(const QString &commandId, 
                const QString &title, 
                std::function<void()> handler);

// 执行命令
void executeCommand(const QString &commandId);
```

### 3.2 快捷键接口

插件可以绑定快捷键：

```cpp
// 注册快捷键
void registerKeybinding(const QString &commandId, 
                    const QString &keybinding);
```

---

## 4. 插件生命周期

| 阶段 | 说明 |
|------|------|
| 加载 | 解析 package.json，注册命令和快捷键 |
| 激活 | 执行插件入口函数 |
| 运行 | 响应用户操作和事件 |
| 停用 | 清理注册的资源 |
| 卸载 | 删除插件文件 |

---

## 5. 已实现插件列表

| 插件 | 功能 |
|------|------|
| (待补充) | 插件市场进一步完善中 |

---

## 6. 插件开发指南

### 6.1 创建插件

1. 创建插件目录：`plugins/my-plugin/`
2. 创建 `package.json`
3. 创建入口文件：`index.js`

### 6.2 调试插件

```cpp
// 在命令面板中调试
// Ctrl+Shift+P -> Developer: Reload Plugins
```

---

## 7. API 参考

### 插件管理器 API

```cpp
class LosPlugUi : public QWidget
{
    // 检查插件列表是否为空
    bool isEmpty() const;
    
    // 获取插件信息
    QList<PluginInfo> getPlugins() const;
}
```

### 插件详情 API

```cpp
class LosPluginDetailUi : public QWidget
{
    // 显示插件详情
    void showDetail(const PluginInfo &info);
}
```