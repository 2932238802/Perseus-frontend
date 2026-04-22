
#include "LosEditorTabUi.h"




namespace LosView
{

    LosEditorTabUi::LosEditorTabUi(QTabWidget *tab_widget, QWidget *parent) : L_tabWidget{tab_widget}, QWidget{parent}
    {
        initConnect();
        initTabBar();
        if (L_tabWidget)
        {
            /*
             * 用自绘按钮代替 Qt 原生关闭按钮,
             * 关闭 setTabsClosable 防止两者同时出现
             */
            L_tabWidget->setTabsClosable(false);
        }
    }

    LosEditorTabUi::~LosEditorTabUi() {}



    /*
     * 关闭标签页
     */
    void LosEditorTabUi::closeTab(int index)
    {
        onTabCloseRequested(index);
    }



    /**
     * @brief closeAllTabs
     * - 关闭所有的标签
     */
    void LosEditorTabUi::closeAllTabs()
    {
        while (L_tabWidget->count() > 0)
        {
            QWidget *wi = L_tabWidget->widget(0);
            L_tabWidget->removeTab(0);
            if (wi)
            {
                wi->deleteLater();
            }
        }
        LOS_pathToUi.clear();
    }



    /*
     * - 保存标签页
     */
    void LosEditorTabUi::saveTab()
    {
        if (L_tabWidget == nullptr)
        {
            return;
        }

        auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
        if (!widget || !widget->isDirty())
        {
            return;
        }
        if (!widget->save())
        {
            ERR("failed to save file", "LosEditorTabUi");
            return;
        }
    }



    /*
     * 保存所有的标签页
     */
    void LosEditorTabUi::saveAllTabs()
    {
        if (nullptr == L_tabWidget)
        {
            ERR("error in saveTab: nullptr", "LosEditorTabUi");
            return;
        }
        for (int i = 0; i < L_tabWidget->count(); i++)
        {
            auto a = qobject_cast<LosEditorUi *>(L_tabWidget->widget(i));
            if (!a || !a->isDirty())
            {
                continue;
            }
            if (!a->save())
            {
                WAR("error in saveAllTabs: save file [" + LOS_pathToUi.key(a) + "] failed!", "LosEditorTabUi")
            }
        }
    }



    /*
     * - tool
     * - 打开文件
     * - 只能打开 非 二进制文件
     */
    void LosEditorTabUi::openFile(const LosModel::LosFilePath &file)
    {
        if (file.isBinary())
        {
            return;
        }
        auto filePath = file.getFilePath();
        checkLspAnsFormat(filePath);

        if (LOS_pathToUi.contains(filePath))
        {
            LosEditorUi *editor = LOS_pathToUi.value(filePath);
            L_tabWidget->setCurrentWidget(editor);
            return;
        }
        LosEditorUi *editor = new LosEditorUi(this);
        auto contextCopy    = QSharedPointer<LosModel::LosFileContext>::create();
        contextCopy->load(filePath);
        auto fileCopy = QSharedPointer<LosModel::LosFilePath>::create(filePath);
        editor->loadContextAndPath(contextCopy, fileCopy);
        int newIndex = L_tabWidget->addTab(editor, QFileInfo(filePath).fileName());
        installCloseButton(newIndex);
        LOS_pathToUi.insert(filePath, editor);
        L_tabWidget->setCurrentWidget(editor);
    }



    /**
     * @brief
     *
     */
    void LosEditorTabUi::formatTab()
    {
        if (getCurEditor())
            getCurEditor()->format();
    }



    /*
     * get
     * 获取当前编辑器
     */
    LosEditorUi *LosEditorTabUi::getCurEditor()
    {
        if (L_tabWidget != nullptr)
        {
            return qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
        }
        return nullptr;
    }



    /*
     * get
     */
    int LosEditorTabUi::tabCount() const
    {
        return L_tabWidget->count();
    }



    /*
     * get
     */
    QString LosEditorTabUi::getCurFilePath() const
    {
        auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
        return widget != nullptr ? LOS_pathToUi.key(widget) : "";
    }



    QStringList LosEditorTabUi::getOpenFiles() const
    {
        return LOS_pathToUi.keys();
    }



    /**
     * @brief getCurEditIndex
     * - 获取当前edit的index
     * - 便于之后的关闭操作
     *
     * @return int
     */
    int LosEditorTabUi::getCurEditIndex() const
    {
        if (L_tabWidget)
        {
            return L_tabWidget->currentIndex();
        }
        return -1;
    }



    /*
     * 关闭 ui 点击
     */
    void LosEditorTabUi::onTabCloseRequested(int index)
    {
        QWidget *wi = L_tabWidget->widget(index);
        if (!wi)
            return;
        LosEditorUi *editor = qobject_cast<LosEditorUi *>(wi);

        /*
         * 补充
         */
        if (editor)
        {
            if (editor->isDirty())
            {
                QString fileName = L_tabWidget->tabText(index);
                fileName.replace(" *", "");
                QMessageBox::StandardButton res = QMessageBox::warning(
                    this, "save tips", QString("file '%1' has been modified. save changes?").arg(fileName),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
                if (res == QMessageBox::Save)
                {
                    if (!editor->save())
                    {
                        ERR(QStringLiteral("error") + "save failed, unable to close the tab!", "LosEditorTabUi");
                        return;
                    }
                }
                else if (res == QMessageBox::Cancel)
                {
                    return;
                }
            }
            QString filePath = LOS_pathToUi.key(editor);
            if (!filePath.isEmpty())
                LOS_pathToUi.remove(filePath);
        }
        L_tabWidget->removeTab(index);
        wi->deleteLater();
    }



    /*
     * 如果编辑器 修改
     * - 使用 ● 作为脏标记,视觉上比 * 更清爽
     */
    void LosEditorTabUi::onEditDirty(const QString &file_path, bool is_dirty)
    {
        if (!LOS_pathToUi.contains(file_path))
            return;

        int index = L_tabWidget->indexOf(LOS_pathToUi[file_path]);
        if (index == -1)
            return;
        QString currentTitle            = L_tabWidget->tabText(index);
        static const QString DIRTY_MARK = QStringLiteral(" \u25CF"); /* ● */

        if (is_dirty && !currentTitle.endsWith(DIRTY_MARK))
        {
            L_tabWidget->setTabText(index, currentTitle + DIRTY_MARK);
        }
        else if (!is_dirty && currentTitle.endsWith(DIRTY_MARK))
        {
            L_tabWidget->setTabText(index, currentTitle.left(currentTitle.length() - DIRTY_MARK.length()));
        }
    }



    /*
     */
    void LosEditorTabUi::onDefineResult(const QString &file_path, int line)
    {
        openFile(file_path);
        if (nullptr != getCurEditor())
        {
            getCurEditor()->gotoLine(line);
        }
    }



    /*
     * 双击
     */
    void LosEditorTabUi::onDoubleClickedOnIssue(const QString &file_path, int line)
    {
        openFile(file_path);
        auto editor = getCurEditor();
        if (editor)
        {
            editor->gotoLine(line);
        }
    }



    /*
     * - 重新 检查
     */
    void LosEditorTabUi::onResetCheck(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &curFile)
    {
        if (L_checkedLanguage.contains(lan))
        {
            L_checkedLanguage.remove(lan);
            if (!curFile.isEmpty())
            {
                checkLspAnsFormat(curFile);
            }
        }
    }



    /*
     * - 点击 上侧的标签页 实现类似的功能
     * - 修复 qobject_cast
     */
    void LosEditorTabUi::onTabClicked(int index)
    {
        if (index < 0 || index >= L_tabWidget->count())
        {
            /*
             * 无效
             */
            return;
        }

        auto *widget = qobject_cast<LosEditorUi *>(L_tabWidget->widget(index));
        if (!widget)
            return;
        widget->setFocus();
        QString filePath = LOS_pathToUi.key(widget);
    }


    



    void LosEditorTabUi::onOpenPlugin(const LosCommon::LosNet_Constants::PluginInfo &info)
    {
        for (int i = 0; i < L_tabWidget->count(); i++)
        {
            if (L_tabWidget->tabToolTip(i) == "plugin:" + info.L_id)
            {
                L_tabWidget->setCurrentIndex(i);
                return;
            }
        }
        auto plugin = new LosPluginDetailUi(L_tabWidget);
        plugin->setPluginInfo(info);
        int newIndex = L_tabWidget->addTab(plugin, "Ext: " + info.L_name);
        L_tabWidget->setTabToolTip(newIndex, "plugin:" + info.L_id);
        installCloseButton(newIndex);
        L_tabWidget->setCurrentIndex(newIndex);
    }



    void LosEditorTabUi::onFileRenamed(const QString &old_path, const QString &new_path)
    {
        if (!LOS_pathToUi.contains(old_path))
        {
            return;
        }

        LosEditorUi *editor = LOS_pathToUi.take(old_path);
        LOS_pathToUi.insert(new_path, editor);
        int index = L_tabWidget->indexOf(editor);
        if (index != -1)
        {
            L_tabWidget->setTabText(index, QFileInfo(new_path).fileName());
        }
        auto context = QSharedPointer<LosModel::LosFileContext>::create();
        context->load(new_path);
        auto fileCopy = QSharedPointer<LosModel::LosFilePath>::create(new_path);
        editor->loadContextAndPath(context, fileCopy);
    }



    /**
     * @brief onGotoLineShort
     * -
     *
     */
    void LosEditorTabUi::onGotoLineShortcut()
    {
        auto editor = getCurEditor();
        if (!editor)
            return;
        int maxLines                               = editor->document()->blockCount();
        LosView::LosGotoLinePopupUi *contentWidget = new LosView::LosGotoLinePopupUi();
        LosView::LosFloatingPanelUi *dialog        = new LosView::LosFloatingPanelUi(contentWidget, true, this);
        connect(contentWidget->getLineEdit(), &QLineEdit::returnPressed, dialog, &QDialog::accept);
        dialog->showAtPosition(editor, LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight);
        contentWidget->getLineEdit()->setFocus();
        if (dialog->exec() == QDialog::Accepted)
        {
            int line = contentWidget->getLineNumber();
            if (line > 0)
            {
                if (line > maxLines)
                {
                    line = maxLines;
                }
                editor->gotoLine(line - 1);
            }
        }
        dialog->deleteLater();
    }



    /**
     * @brief onFindShortcut
     * 搜索的快捷键
     */
    void LosEditorTabUi::onFindShortcut()
    {
        auto edit                              = getCurEditor();
        LosView::LosFindPopupUi *contentWidget = new LosView::LosFindPopupUi();
        LosView::LosFloatingPanelUi *dialog    = new LosView::LosFloatingPanelUi(contentWidget, true, this);
        connect(contentWidget->getEdit(), &QLineEdit::returnPressed, dialog, &QDialog::accept);
        dialog->showAtPosition(edit, LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight);
        contentWidget->getEdit()->setFocus();
        if (dialog->exec() == QDialog::Accepted)
        {
            INF("1", "1");
        }
        dialog->deleteLater();
    }



    /**
     * @brief initConnect
     * - 初始化 信号槽
     *
     */
    void LosEditorTabUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(L_tabWidget, &QTabWidget::tabCloseRequested, this, &LosEditorTabUi::onTabCloseRequested);
        /*
         * 收到 定义 结果 就去处理
         */
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_definition, this, &LosEditorTabUi::onDefineResult);
        connect(&router, &LosCore::LosRouter::_cmd_gotoFile, this, &LosEditorTabUi::onDoubleClickedOnIssue);
        connect(&router, &LosCore::LosRouter::_cmd_codeFormat, this, &LosEditorTabUi::formatTab);
        connect(&router, &LosCore::LosRouter::_cmd_fileDirty, this, &LosEditorTabUi::onEditDirty);
        connect(&router, &LosCore::LosRouter::_cmd_openPluginDetail, this, &LosEditorTabUi::onOpenPlugin);
        connect(&router, &LosCore::LosRouter::_cmd_fileRenamed, this, &LosEditorTabUi::onFileRenamed);
        if (L_tabWidget)
        {
            connect(L_tabWidget, &QTabWidget::currentChanged, this, &LosEditorTabUi::onTabClicked);
        }
    }


    /**
     * @brief initTabBar
     * - 右键悬浮标签的 弹窗
     */
    void LosEditorTabUi::initTabBar()
    {
        auto *tabBar = L_tabWidget->findChild<QTabBar *>();
        if (!tabBar)
            return;

        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tabBar, &QTabBar::customContextMenuRequested, this,
                [this, tabBar](const QPoint &pos)
                {
                    int index = tabBar->tabAt(pos);
                    if (index < 0)
                        return;
                    QMenu menu;

                    QAction *pin           = new QAction(LosCommon::LosEditorTableUi_Constants::ACTION_PIN);
                    QAction *copyLocalPath = new QAction(LosCommon::LosEditorTableUi_Constants::ACTION_COPY_LOCAL_PATH);
                    menu.addAction(pin);
                    menu.addAction(copyLocalPath);
                    QAction *ac  = menu.exec(tabBar->mapToGlobal(pos));
                    QWidget *tab = L_tabWidget->widget(index);
                    if (ac == pin)
                    {
                        tabBar->moveTab(index, 0);
                        QString curText = tabBar->tabText(0);
                        if (!curText.startsWith(tr("[pin]")))
                        {
                            tabBar->setTabText(0, tr("[pin]") + curText);
                            tab->setProperty("isPinned", true);
                        }
                    }
                    else if (ac == copyLocalPath)
                    {
                        LosEditorUi *editor = qobject_cast<LosEditorUi *>(tab);
                        if (!editor)
                            return;
                        QString filePath = LOS_pathToUi.key(editor);
                        QClipboard *cp   = QApplication::clipboard();
                        cp->setText(filePath);
                    }
                });
    }



    /**
     * @brief installCloseButton
     * - 为 tab 自绘关闭按钮
     * - 点击后通过 onTabCloseRequested 走统一的关闭流程(包括脏数据提示)
     */
    void LosEditorTabUi::installCloseButton(int index)
    {
        auto *tabBar = L_tabWidget ? L_tabWidget->findChild<QTabBar *>() : nullptr;
        if (!tabBar || index < 0 || index >= tabBar->count())
            return;

        auto *btn = new QToolButton(tabBar);
        btn->setObjectName("tab_close_btn");
        btn->setText(QStringLiteral("\u00D7"));
        btn->setToolTip(tr("Close"));
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setFixedSize(18, 18);
        btn->setStyleSheet(LosCommon::LosEditorTableUi_Constants::CLOSE_BTN_STYLE);

        connect(btn, &QToolButton::clicked, this,
                [this, btn]()
                {
                    auto *tb = L_tabWidget ? L_tabWidget->findChild<QTabBar *>() : nullptr;
                    if (!tb)
                        return;
                    for (int i = 0; i < tb->count(); i++)
                    {
                        if (tb->tabButton(i, QTabBar::RightSide) == btn)
                        {
                            onTabCloseRequested(i);
                            return;
                        }
                    }
                });
        tabBar->setTabButton(index, QTabBar::RightSide, btn);
    }



    /**
     * @brief initShortCut
     * - 初始化 快捷键
     */
    void LosEditorTabUi::initShortCut()
    {
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::GOTO_LINE, this,
                                                    [this]() { this->onGotoLineShortcut(); });
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::SEARCH_FIND, this,
                                                    [this]() { this->onFindShortcut(); });
    }



    /**
     * @brief checkLspAnsFormat
     * - 检查 lsp 和 格式化
     *
     * @param file_path
     */
    void LosEditorTabUi::checkLspAnsFormat(const QString &file_path)
    {
        auto lang = LosCommon::CheckLang(file_path);
        if (L_checkedLanguage.contains(lang))
            return;

        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                lang, LosCommon::LosToolChain_Constants::LosTool::CLANGD);
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                lang, LosCommon::LosToolChain_Constants::LosTool::CLANG_FORMAT);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::CXX);
            return;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                lang, LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER);
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                lang, LosCommon::LosToolChain_Constants::LosTool::CARGO);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::RUST);
            return;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
        {
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                lang, LosCommon::LosToolChain_Constants::LosTool::PYRIGHT);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::PYTHON);
            return;
        }
        default:
            break;
        }

        if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
        {
            emit LosCore::LosRouter::instance()._cmd_checkSingleTool(
                LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP);
        }
    }


} /* namespace LosView */
