// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosCommandUi.h"
#include "common/constants/ConstantsClass/LosEditorTabUiClass.h"
#include "common/constants/ConstantsNum/LosCommandUiNum.h"
#include "common/constants/ConstantsNum/LosSessionNum.h"
#include "common/util/NumberToCommandsKind.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosCommandUi_style.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QVBoxLayout>

namespace LosView
{
    /**
     * @brief Construct a new Los Command Ui:: Los Command Ui object
     *
     * @param parent
     */
    LosCommandUi::LosCommandUi(QWidget *parent) : QDialog(parent)
    {
        initStyle();
        initConnect();
        initScanLocalPlugins();
    }



    /**
     * @brief regis
     *
     * @param display
     * @param cmd_info
     */
    void LosCommandUi::regis(const QString &display, const LosCommon::LosCommandUi_Constants::CommandsInfo &cmd_info)
    {
        L_commands.insert(display, cmd_info);
        L_lists->addItem(display);
    }



    /**
     * @brief 显示搜索的 框
     *
     *
     * @param anchor
     */
    void LosCommandUi::showPalette(QWidget *anchor)
    {
        L_searchBox->clear();
        onSearchTextChanged("");
        if (L_lists->count() > 0)
        {
            L_lists->setCurrentRow(0);
        }

        applyZoomScale(); // 打开前同步全局缩放（尺寸 + 字体）
        if (anchor)
        {
            const QRect targetRect = anchor->rect();
            QPoint globalTopLeft   = anchor->mapToGlobal(targetRect.topLeft());
            move(globalTopLeft + QPoint((targetRect.width() - width()) / 2, (targetRect.height() - height()) / 2 - 40));
        }
        show();
        L_searchBox->setFocus();
    }



    /**
     * @brief 初始化
     */
    void LosCommandUi::initStyle()
    {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(15, 15, 15, 15);
        QFrame *container = new QFrame(this);
        container->setObjectName("PaletteContainer");
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(0, 0, 0, 180));
        shadow->setOffset(0, 5);
        container->setGraphicsEffect(shadow);
        QVBoxLayout *containerLayout = new QVBoxLayout(container);
        containerLayout->setContentsMargins(0, 0, 0, 0);
        containerLayout->setSpacing(0);
        L_searchBox = new QLineEdit(container);
        L_lists     = new QListWidget(container);
        L_searchBox->setPlaceholderText("type a command ...");
        L_searchBox->installEventFilter(this);
        L_lists->setFocusPolicy(Qt::NoFocus);
        containerLayout->addWidget(L_searchBox);
        containerLayout->addWidget(L_lists);
        mainLayout->addWidget(container);
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosCommandUi_styleTemplate(),
                                                                               LosCore::LosThemeManager::instance().currentTheme());
        this->setStyleSheet(qss);
        applyZoomScale();
    }



    /*
     * - 初始化链接
     */
    void LosCommandUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(L_searchBox, &QLineEdit::textChanged, this, &LosCommandUi::onSearchTextChanged);
        connect(L_lists, &QListWidget::itemClicked, this, &LosCommandUi::executeSelectedItem);
        connect(&router, &LosCore::LosRouter::_cmd_net_PluginPath, this, &LosCommandUi::onPluginPath);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this,
                [this](const QString &name)
                {
                    const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::LosCommandUi_styleTemplate(), name);
                    this->setStyleSheet(qss);
                    applyZoomScale();
                });
    }



    /**
     * @brief applyZoomScale
     * 按全局应用字体相对设计基准 DEFAULT_FONT_SIZE 等比缩放
     * - 面板固定尺寸
     * - 搜索框 / 列表字体 样式表不再写死 font-size 此处强制同步
     */
    void LosCommandUi::applyZoomScale()
    {
        const int baseSize = LosCommon::LosSession_Constants::DEFAULT_FONT_SIZE;
        const int curSize  = QApplication::font().pointSize();
        if (baseSize <= 0 || curSize <= 0)
        {
            return;
        }
        const double scale = static_cast<double>(curSize) / static_cast<double>(baseSize);
        setFixedSize(qRound(LosCommon::LosCommandUi_Constants::PALETTE_WIDTH * scale),
                     qRound(LosCommon::LosCommandUi_Constants::PALETTE_HEIGHT * scale));

        const QFont appFont = QApplication::font();
        setFont(appFont);
        if (L_searchBox)
        {
            L_searchBox->setFont(appFont);
        }
        if (L_lists)
        {
            L_lists->setFont(appFont);
        }
    }



    /**
     * @brief initScanLocalPlugins
     * 扫描 当地的插件
     */
    void LosCommandUi::initScanLocalPlugins()
    {
        QString extDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.perseus/extensions/";
        QDir dir(extDir);
        if (!dir.exists())
            return;

        QStringList pluginFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &folderName : pluginFolders)
        {
            onPluginPath(dir.absoluteFilePath(folderName));
        }
    }



    /**
     * @brief 文本修改
     *
     * @param text
     */
    void LosCommandUi::onSearchTextChanged(const QString &text)
    {
        for (int i = 0; i < L_lists->count(); i++)
        {
            QListWidgetItem *item = L_lists->item(i);
            /*
             * 小写
             */
            bool isMatch = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!isMatch);
        }
        for (int i = 0; i < L_lists->count(); ++i)
        {
            if (!L_lists->item(i)->isHidden())
            {
                L_lists->setCurrentRow(i);
                break;
            }
        }
    }



    /**
     * @brief  执行 选中的 item
     */
    void LosCommandUi::executeSelectedItem()
    {
        auto item = L_lists->currentItem();
        if (!item || item->isHidden())
        {
            return;
        }
        auto scripInfo = L_commands.value(item->text());
        hide();
        if (scripInfo.kind == LosCommon::LosCommandUi_Constants::CommandsKind::SCRIPT)
        {
            emit LosCore::LosRouter::instance()._cmd_command_exeScript(scripInfo.L_scriptPath, scripInfo.L_params);
        }
        else
        {
            WAR("unkown script info kind", "LosCommandUi");
        }
    }



    /*
     * - 解析 这个json
     * {
     * "id": "perseus-rust",
     * "name": "Rust Support For create and build",
     * "version": "1.0.0",
     * "description": "provides Cargo project creation and building support",
     * "contributes": {
     * "commands": {
     * "rust.create.linux": {
     * "kind": 1,
     * "scripts": "./scripts/create.sh",
     * "params": [
     * // "project_name"
     * ]
     * },
     * "rust.create.windows": {
     * "kind": 1,
     * "scripts": "./scripts/create.bat",
     * "params": [
     * "project_name"
     * ]
     * },
     * "rust.build.linux": {
     * "kind": 1,
     * "scripts": "./scripts/build.sh",
     * "params": []
     * }
     * }
     * }
     * }
     */
    void LosCommandUi::onPluginPath(const QString &plugin_path)
    {
        /*
         * plugin_path =  /home/losangelous/.perseus/extensions/rust-extension
         * INF(plugin_path, "LosCommandUI");
         */
        QFileInfo file(plugin_path);
        if (!file.exists())
        {
            ERR("file:" + plugin_path + " is not exist!", "LosCommandUi");
            return;
        }

        QDir dir(plugin_path);
        QString jsonPath = dir.filePath("package.json");
        QFile f(jsonPath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ERR("open file failed!", "LosCommandUi");
            return;
        }

        QJsonDocument dic = QJsonDocument::fromJson(f.readAll());
        if (!dic.isObject())
        {
            ERR("dic is not QJsonDocument!", "LosCommandUi");
            return;
        }

        QJsonObject obj = dic.object();
        if (!obj.contains("contributes"))
        {
            ERR("obj is not contain contributes", "LosCommandUi");
            return;
        }

        QJsonObject contributesObj = obj["contributes"].toObject();
        if (!contributesObj.contains("commands"))
        {
            ERR("contributesObj is not contain commands", "LosCommandUi");
            return;
        }
        QJsonObject commandsObj = contributesObj["commands"].toObject();
        for (auto i = commandsObj.begin(); i != commandsObj.end(); i++)
        {
            QString cmdName = i.key();
#ifdef Q_OS_WIN
            if (cmdName.endsWith(".linux"))
                continue;
#else
            if (cmdName.endsWith(".windows"))
                continue;
#endif
            QJsonObject cmdValue = i.value().toObject();
            /*
             * 如果 没有 这两个 基本的 就 下一个
             */
            if (!cmdValue.contains("kind") || !cmdValue.contains("scripts"))
            {
                WAR("Command missing kind or scripts: " + cmdName, "LosCommandUi");
                continue;
            }
            auto kind = LosCommon::NumberToCommandsKind(cmdValue["kind"].toInt());

            switch (kind)
            {
            case LosCommon::LosCommandUi_Constants::CommandsKind::SCRIPT:
            {
                struct LosCommon::LosCommandUi_Constants::CommandsInfo info
                {
                    .kind = kind
                };
                QString scriptRelativePath = cmdValue["scripts"].toString();
                if (scriptRelativePath.startsWith("./"))
                {
                    scriptRelativePath = scriptRelativePath.mid(2);
                }
                QString scriptAbsolutePath = QDir(plugin_path).filePath(scriptRelativePath);
                info.L_scriptPath          = scriptAbsolutePath;

                QStringList paramsList;
                if (!cmdValue.contains("params"))
                    continue;
                QJsonArray params = cmdValue["params"].toArray();
                for (auto param : params)
                {
                    if (param.isString())
                    {
                        paramsList.append(param.toString());
                    }
                }
                info.L_params = std::move(paramsList);
                regis(cmdName, info);
                break;
            }
            case LosCommon::LosCommandUi_Constants::CommandsKind::THEME:
            {
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }



    /**
     * @brief 过滤 掉 一些 值
     *
     * @param watched
     * @param event
     * @return true
     * @return false
     */
    bool LosCommandUi::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched == L_searchBox && event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int currentRow      = L_lists->currentRow();

            if (keyEvent->key() == Qt::Key_Down)
            {
                for (int i = currentRow + 1; i < L_lists->count(); ++i)
                {
                    if (!L_lists->item(i)->isHidden())
                    {
                        L_lists->setCurrentRow(i);
                        break;
                    }
                }
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Up)
            {
                for (int i = currentRow - 1; i >= 0; --i)
                {
                    if (!L_lists->item(i)->isHidden())
                    {
                        L_lists->setCurrentRow(i);
                        break;
                    }
                }
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                executeSelectedItem();
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Escape)
            {
                hide();
                return true;
            }
            else if ((keyEvent->key() == Qt::Key_F || keyEvent->key() == Qt::Key_H || keyEvent->key() == Qt::Key_G) &&
                     keyEvent->modifiers().testFlag(Qt::ControlModifier) && !keyEvent->modifiers().testFlag(Qt::ShiftModifier))
            {
                // 指令面板打开期间按 Ctrl+F / Ctrl+H / Ctrl+G：关闭面板并切换到对应弹窗
                LosCommon::LosEditorTableUi_Constants::PopupKind kind;
                if (keyEvent->key() == Qt::Key_F)
                {
                    kind = LosCommon::LosEditorTableUi_Constants::PopupKind::Find;
                }
                else if (keyEvent->key() == Qt::Key_H)
                {
                    kind = LosCommon::LosEditorTableUi_Constants::PopupKind::Replace;
                }
                else
                {
                    kind = LosCommon::LosEditorTableUi_Constants::PopupKind::GotoLine;
                }
                hide();
                emit LosCore::LosRouter::instance()._cmd_commandPaletteSwitchRequested(kind);
                return true;
            }
        }
        return QDialog::eventFilter(watched, event);
    }



    /**
     * @brief focusOutEvent
     *
     * @param event
     */
    void LosCommandUi::focusOutEvent(QFocusEvent *event)
    {
        hide();
        QDialog::focusOutEvent(event);
    }



    /**
     * @brief changeEvent
     *
     * @param event
     */
    void LosCommandUi::changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::FontChange || event->type() == QEvent::ApplicationFontChange)
        {
            applyZoomScale();
        }
        QDialog::changeEvent(event);
    }



    /**
     * @brief event
     *
     * @param event
     * @return true
     * @return false
     */
    bool LosCommandUi::event(QEvent *event)
    {
        if (event->type() == QEvent::WindowDeactivate)
        {
            hide();
        }
        return QDialog::event(event);
    }
} /* namespace LosView */