
#include "LosCommandUi.h"

namespace LosView
{

    LosCommandUi::LosCommandUi(QWidget *parent) : QDialog(parent)
    {
        initStyle();
        initConnect();
        initScanLocalPlugins();
    }


    void LosCommandUi::regis(const QString &display, const LosCommon::LosCommandUi_Constants::CommandsInfo &cmd_info)
    {
        L_commands.insert(display, cmd_info);
        L_lists->addItem(display);
    }


    /*
     * - 显示搜索的 框
     */
    void LosCommandUi::showPalette()
    {
        L_searchBox->clear();
        onSearchTextChanged("");
        if (L_lists->count() > 0)
        {
            L_lists->setCurrentRow(0);
        }

        if (parentWidget())
        {
            QPoint globalPos = parentWidget()->mapToGlobal(QPoint(0, 0));
            int x            = globalPos.x() + (parentWidget()->width() - width()) / 2;
            int y            = globalPos.y() + 100;
            move(x, y);
        }
        show();
        L_searchBox->setFocus();
    }



    /*
     * - 初始化 央视
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
        setFixedSize(630, 380);
        this->setStyleSheet(LosStyle::LosCommandUi_style());
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
    }



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



    /*
     * - 文本的 改动
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



    /*
     * - 执行 选中的 item
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



    /*
     * - 过滤掉 一些 按键
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
        }
        return QDialog::eventFilter(watched, event);
    }



    void LosCommandUi::focusOutEvent(QFocusEvent *event)
    {
        hide();
        QDialog::focusOutEvent(event);
    }



    bool LosCommandUi::event(QEvent *event)
    {
        if (event->type() == QEvent::WindowDeactivate)
        {
            hide();
        }
        return QDialog::event(event);
    }


} /* namespace LosView */