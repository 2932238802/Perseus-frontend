// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosEditorTabUi.h"
#include "common/constants/ConstantsStr/LosEditorTableUiStr.h"
#include "common/constants/ConstantsStr/ShortCut.h"
#include "common/util/CheckLang.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "models/LosFileContext/LosFileContext.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosFloatingPanelUi/LosFindPopupUi/LosFindPopupUi.h"
#include "view/LosFloatingPanelUi/LosFloatingPanelUi.h"
#include "view/LosFloatingPanelUi/LosGotoLinePopupUi/LosGotoLinePopupUi.h"
#include "view/LosPluginDetailUi/LosPluginDetailUi.h"
#include "view/LosPreview/LosMDPreview/LosMDPreview.h"
#include "view/LosPreview/LosPreview.h"
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QSharedPointer>
#include <QStringLiteral>
#include <QTabBar>
#include <QToolButton>
#include <qfileinfo.h>

namespace LosView
{
    /**
     * @brief Construct a new Los Editor Tab Ui:: Los Editor Tab Ui object
     *
     * @param tab_widget
     * @param parent
     */
    LosEditorTabUi::LosEditorTabUi(QTabWidget *tab_widget, QWidget *parent) : L_tabWidget{tab_widget}, QWidget{parent}
    {
        initConnect();
        initTabBar();
        initShortCut();
        if (L_tabWidget)
        {
            L_tabWidget->setTabsClosable(false);
        }
    }
    LosEditorTabUi::~LosEditorTabUi() {}



    /**
     * @brief closeTab 关闭标签
     *
     * @param index
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



    /**
     * @brief saveTab
     * 标签页的保存
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
        auto filePath = file.getAbsoluteFilePath();
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
        emit LosCore::LosRouter::instance()._cmd_fileChanged(file.getAbsoluteFilePath());
    }



    /**
     * @brief 格式化内容
     */
    void LosEditorTabUi::formatTab()
    {
        auto widget = getCurEditor();
        if (widget != nullptr)
        {
            auto editor = qobject_cast<LosEditorUi *>(widget);
            editor->format();
        }
    }



    /**
     * @brief getCurEditor
     * 获取当前编辑器
     *
     * @return LosEditorUi*
     */
    QWidget *LosEditorTabUi::getCurEditor(LosCommon::LosEditorTableUi_Constants::EditorType type)
    {
        if (type == LosCommon::LosEditorTableUi_Constants::EditorType::CODE)
        {
            return qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
        }
        else if (type == LosCommon::LosEditorTableUi_Constants::EditorType::PREVIEW)
        {
            return qobject_cast<LosPreview *>(L_tabWidget->currentWidget());
        }
        return nullptr;
    }



    /**
     * @brief tabCount
     *
     * @return int
     */
    int LosEditorTabUi::tabCount() const
    {
        return L_tabWidget->count();
    }



    /**
     * @brief getCurFilePath
     *
     * @return QString
     */
    QString LosEditorTabUi::getCurFilePath() const
    {
        QWidget *cur = L_tabWidget->currentWidget();
        // 当前是源码 tab: 从 LOS_pathToUi 反查路径
        if (auto *editor = qobject_cast<LosEditorUi *>(cur))
        {
            return LOS_pathToUi.key(editor);
        }
        // 当前是预览 tab: 从 LOS_pathToPreview 反查路径
        if (auto *preview = qobject_cast<LosPreview *>(cur))
        {
            return LOS_pathToPreview.key(preview);
        }
        return "";
    }



    /**
     * @brief getOpenFiles
     *
     * @return QStringList
     */
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



    /**
     * @brief onTabCloseRequested
     * 关闭 ui 点击
     * @param index
     */
    void LosEditorTabUi::onTabCloseRequested(int index)
    {
        QWidget *wi = L_tabWidget->widget(index);
        if (!wi)
            return;
        LosEditorUi *editor = qobject_cast<LosEditorUi *>(wi);
        // 补充
        if (editor)
        {
            if (editor->isDirty())
            {
                QString fileName = L_tabWidget->tabText(index);
                fileName.replace(" *", "");
                QMessageBox::StandardButton res =
                    QMessageBox::warning(this, "save tips", QString("file '%1' has been modified. save changes?").arg(fileName),
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
            {
                LOS_pathToUi.remove(filePath);
            }
        }

        L_tabWidget->removeTab(index);
        wi->deleteLater();
    }



    /**
     * @brief
     *
     * @param file_path
     * @param is_dirty
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



    /**
     * @brief onDefineResult 定义跳转
     *
     * @param file_path
     * @param line
     */
    void LosEditorTabUi::onDefineResult(const QString &file_path, int line)
    {
        openFile(file_path);
        auto widget = getCurEditor();
        if (nullptr != widget)
        {
            auto editor = qobject_cast<LosEditorUi *>(widget);
            editor->gotoLine(line);
        }
    }



    /**
     * @brief 错误信息跳转
     *
     * @param file_path
     * @param line
     */
    void LosEditorTabUi::onDoubleClickedOnIssue(const QString &file_path, int line)
    {
        openFile(file_path);
        auto widget = getCurEditor();
        if (widget != nullptr)
        {
            auto editer = qobject_cast<LosEditorUi *>(widget);
            editer->gotoLine(line);
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



    /**
     * @brief onTabClicked
     * 点击 上侧的标签页 实现类似的功能
     * 修复 qobject_cast
     * @param index
     */
    void LosEditorTabUi::onTabClicked(int index)
    {
        if (index < 0 || index >= L_tabWidget->count())
        {
            return;
        }
        auto *widget = qobject_cast<LosEditorUi *>(L_tabWidget->widget(index));
        if (!widget)
            return;
        widget->setFocus();
        QString filePath = LOS_pathToUi.key(widget);
        emit LosCore::LosRouter::instance()._cmd_fileChanged(filePath);
    }



    /**
     * @brief onOpenPlugin
     * 插件界面的打开
     *
     * @param info
     */
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



    /**
     * @brief onFileRenamed
     *
     * @param old_path
     * @param new_path
     */
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
     * 跳转到指定的行
     */
    void LosEditorTabUi::onGotoLineShortcut()
    {
        showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind::GotoLine);
    }



    /**
     * @brief onFindShortcut
     * 搜索的快捷键
     */
    void LosEditorTabUi::onFindShortcut()
    {
        showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind::Find);
    }



    /**
     * @brief onReplaceShortcut
     * 替换的快捷键
     */
    void LosEditorTabUi::onReplaceShortcut()
    {
        showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind::Replace);
    }



    /**
     * @brief onFindNextShortcut
     * F3 查找下一个（基于编辑器内保留的搜索状态）
     */
    void LosEditorTabUi::onFindNextShortcut()
    {
        auto widget = getCurEditor();
        if (!widget)
        {
            return;
        }
        qobject_cast<LosEditorUi *>(widget)->searchNext();
    }



    /**
     * @brief onFindPreviousShortcut
     * Shift+F3 查找上一个（基于编辑器内保留的搜索状态）
     */
    void LosEditorTabUi::onFindPreviousShortcut()
    {
        auto widget = getCurEditor();
        if (!widget)
        {
            return;
        }
        qobject_cast<LosEditorUi *>(widget)->searchPrevious();
    }



    /**
     * @brief showFindPopup
     * 打开查找 / 替换 / 跳转行弹窗。
     * 弹窗打开期间按 Ctrl+F / Ctrl+H / Ctrl+G 会在三种弹窗之间互相切换，
     * 切换以循环方式完成（关闭当前弹窗后继续打开目标弹窗）
     *
     * @param kind 初始弹窗类型
     */
    void LosEditorTabUi::showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind kind)
    {
        while (true)
        {
            if (kind == LosCommon::LosEditorTableUi_Constants::PopupKind::GotoLine)
            {
                auto widget = getCurEditor();
                if (!widget)
                {
                    return;
                }
                auto *editor                               = qobject_cast<LosEditorUi *>(widget);
                int maxLines                               = editor->document()->blockCount();
                LosView::LosGotoLinePopupUi *contentWidget = new LosView::LosGotoLinePopupUi();
                LosView::LosFloatingPanelUi *dialog        = new LosView::LosFloatingPanelUi(contentWidget, true, this);
                connect(contentWidget->getLineEdit(), &QLineEdit::returnPressed, dialog, &QDialog::accept);
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findPopupSwitchRequested, contentWidget,
                        [this, dialog](bool with_replace)
                        {
                            L_pendingPopupKind = with_replace ? LosCommon::LosEditorTableUi_Constants::PopupKind::Replace
                                                              : LosCommon::LosEditorTableUi_Constants::PopupKind::Find;
                            dialog->reject();
                        });
                dialog->showAtPosition(editor, LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight);
                contentWidget->getLineEdit()->setFocus();
                const bool accepted = dialog->exec() == QDialog::Accepted;
                dialog->deleteLater();
                if (accepted)
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
            }
            else
            {
                auto widget = getCurEditor();
                if (!widget)
                {
                    return;
                }
                auto *edit                             = qobject_cast<LosEditorUi *>(widget);
                LosView::LosFindPopupUi *contentWidget = new LosView::LosFindPopupUi();
                LosView::LosFloatingPanelUi *dialog    = new LosView::LosFloatingPanelUi(contentWidget, true, this);
                contentWidget->setReplaceVisible(kind == LosCommon::LosEditorTableUi_Constants::PopupKind::Replace);
                dialog->showAtPosition(edit, LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight);
                contentWidget->getEdit()->setFocus();

                auto refreshSearch = [edit, contentWidget]()
                {
                    const QString text = contentWidget->getInput();
                    if (text.isEmpty())
                    {
                        edit->clearSearch();
                        contentWidget->setMatchInfo(0, 0);
                        return;
                    }
                    edit->updateSearch(text, contentWidget->getFlags(), contentWidget->isRegex());
                    contentWidget->setMatchInfo(edit->searchCurrentIndex(), edit->searchMatchCount());
                };

                connect(contentWidget->getEdit(), &QLineEdit::textChanged, contentWidget, refreshSearch);
                connect(contentWidget, &LosView::LosFindPopupUi::searchOptionsChanged, contentWidget, refreshSearch);
                connect(contentWidget, &LosView::LosFindPopupUi::resizeRequested, dialog, &QWidget::adjustSize);
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findNextRequested, contentWidget,
                        [edit, contentWidget]()
                        {
                            if (edit->searchNext())
                            {
                                contentWidget->setMatchInfo(edit->searchCurrentIndex(), edit->searchMatchCount());
                            }
                        });
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findPreviousRequested, contentWidget,
                        [edit, contentWidget]()
                        {
                            if (edit->searchPrevious())
                            {
                                contentWidget->setMatchInfo(edit->searchCurrentIndex(), edit->searchMatchCount());
                            }
                        });
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findReplaceRequested, contentWidget,
                        [edit, contentWidget]()
                        {
                            if (edit->replaceCurrent(contentWidget->getReplaceInput()))
                            {
                                contentWidget->setMatchInfo(edit->searchCurrentIndex(), edit->searchMatchCount());
                            }
                        });
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findReplaceAllRequested, contentWidget,
                        [edit, contentWidget]()
                        {
                            edit->replaceAll(contentWidget->getReplaceInput());
                            contentWidget->setMatchInfo(edit->searchCurrentIndex(), edit->searchMatchCount());
                        });
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_gotoLinePopupSwitchRequested, contentWidget,
                        [this, dialog]()
                        {
                            L_pendingPopupKind = LosCommon::LosEditorTableUi_Constants::PopupKind::GotoLine;
                            dialog->reject();
                        });

                QString prefill;
                const QTextCursor cur = edit->textCursor();
                if (cur.hasSelection())
                {
                    prefill = cur.selectedText();
                }
                else if (!edit->getLastSearchText().isEmpty())
                {
                    prefill = edit->getLastSearchText();
                }
                else
                {
                    prefill = edit->getWordUnderCursor();
                }
                if (!prefill.isEmpty())
                {
                    contentWidget->getEdit()->setText(prefill);
                    contentWidget->getEdit()->selectAll();
                }

                dialog->exec();
                dialog->deleteLater();
            }

            const LosCommon::LosEditorTableUi_Constants::PopupKind next = L_pendingPopupKind;
            L_pendingPopupKind                                          = LosCommon::LosEditorTableUi_Constants::PopupKind::None;
            if (next == LosCommon::LosEditorTableUi_Constants::PopupKind::None)
            {
                return;
            }
            kind = next;
        }
    }



    /**
     * @brief 切换当前标签页的预览状态
     *
     * @param absolute_file_path
     */
    void LosEditorTabUi::onTogglePreview(const QString &absolute_file_path)
    {
        if (LosCommon::CheckLang(absolute_file_path) != LosCommon::LosToolChain_Constants::LosLanguage::MARKDOWN)
        {
            return;
        }
        if (getCurEditor(LosCommon::LosEditorTableUi_Constants::EditorType::PREVIEW) != nullptr)
        {
            if (LOS_pathToUi.contains(absolute_file_path))
            {
                L_tabWidget->setCurrentWidget(LOS_pathToUi.value(absolute_file_path));
            }
            return;
        }
        LosEditorUi *editor = LOS_pathToUi.value(absolute_file_path, nullptr);
        if (editor == nullptr)
        {
            return;
        }
        LosPreview *preview = nullptr;
        if (LOS_pathToPreview.contains(absolute_file_path))
        {
            preview = LOS_pathToPreview.value(absolute_file_path);
        }
        else
        {
            preview      = new LosMDPreview(this);
            int newIndex = L_tabWidget->addTab(preview, QFileInfo(absolute_file_path).fileName() + " (Preview)");
            installCloseButton(newIndex);
            LOS_pathToPreview.insert(absolute_file_path, preview);
        }
        preview->render(editor->toPlainText());
        L_tabWidget->setCurrentWidget(preview);
    }



    /**
     * @brief initConnect
     * - 初始化 信号槽
     */
    void LosEditorTabUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(L_tabWidget, &QTabWidget::tabCloseRequested, this, &LosEditorTabUi::onTabCloseRequested);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_definition, this, &LosEditorTabUi::onDefineResult);
        connect(&router, &LosCore::LosRouter::_cmd_gotoFile, this, &LosEditorTabUi::onDoubleClickedOnIssue);
        connect(&router, &LosCore::LosRouter::_cmd_codeFormat, this, &LosEditorTabUi::formatTab);
        connect(&router, &LosCore::LosRouter::_cmd_fileDirty, this, &LosEditorTabUi::onEditDirty);
        connect(&router, &LosCore::LosRouter::_cmd_openPluginDetail, this, &LosEditorTabUi::onOpenPlugin);
        connect(&router, &LosCore::LosRouter::_cmd_fileRenamed, this, &LosEditorTabUi::onFileRenamed);
        connect(&router, &LosCore::LosRouter::LosRouter::_cmd_togglePreview, this, &LosEditorTabUi::onTogglePreview);
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
                                                    [this]()
                                                    {
                                                        INF("ctrl + g", "LosEditorTabUi");
                                                        this->onGotoLineShortcut();
                                                    });
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::SEARCH_FIND, this, [this]() { this->onFindShortcut(); });
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::SEARCH_REPLACE, this, [this]() { this->onReplaceShortcut(); });
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::SEARCH_NEXT, this, [this]() { this->onFindNextShortcut(); });
        LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::SEARCH_PREVIOUS, this, [this]() { this->onFindPreviousShortcut(); });
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
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::CLANGD);
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::CLANG_FORMAT);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::CXX);
            return;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER);
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::CARGO);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::RUST);
            return;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
        {
            emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::PYRIGHT);
            L_checkedLanguage.insert(LosCommon::LosToolChain_Constants::LosLanguage::PYTHON);
            return;
        }
        default:
            break;
        }

        if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
        {
            emit LosCore::LosRouter::instance()._cmd_checkSingleTool(LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP);
        }
    }


} /* namespace LosView */
