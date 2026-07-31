// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "view/LosFindPopupManager/LosFindPopupManager.h"
#include "core/LosRouter/LosRouter.h"
#include "view/LosCommandUi/LosCommandUi.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosFloatingPanelUi/LosFindPopupUi/LosFindPopupUi.h"
#include "view/LosFloatingPanelUi/LosFloatingPanelUi.h"
#include "view/LosFloatingPanelUi/LosGotoLinePopupUi/LosGotoLinePopupUi.h"

#include <QLineEdit>
#include <QTextCursor>


namespace LosView
{

    /**
     * @brief Construct a new Los Find Popup Manager:: Los Find Popup Manager object
     *
     * @param tab 所属编辑器标签页，用于获取当前编辑器
     * @param parent
     */
    LosFindPopupManager::LosFindPopupManager(LosEditorTabUi *tab, QObject *parent) : L_tab{tab}, QObject{parent}
    {
        // 指令面板打开期间按 Ctrl+F / Ctrl+H / Ctrl+G：切换到对应弹窗
        connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_commandPaletteSwitchRequested, this,
                [this](LosCommon::LosEditorTableUi_Constants::PopupKind kind) { showFindPopup(kind); });
    }



    /**
     * @brief findNext
     * F3 查找下一个（基于编辑器内保留的搜索状态）
     */
    void LosFindPopupManager::findNext()
    {
        auto widget = L_tab->getCurEditor();
        if (!widget)
        {
            return;
        }
        qobject_cast<LosEditorUi *>(widget)->searchNext();
    }



    /**
     * @brief findPrevious
     * Shift+F3 查找上一个（基于编辑器内保留的搜索状态）
     */
    void LosFindPopupManager::findPrevious()
    {
        auto widget = L_tab->getCurEditor();
        if (!widget)
        {
            return;
        }
        qobject_cast<LosEditorUi *>(widget)->searchPrevious();
    }



    /**
     * @brief showCommandPalette
     * Ctrl+Shift+P 打开指令面板（懒创建，居中于编辑器区域）
     */
    void LosFindPopupManager::showCommandPalette()
    {
        if (LOS_cmdPalette == nullptr)
        {
            LOS_cmdPalette = new LosCommandUi(L_tab);
        }
        auto *editor = qobject_cast<LosEditorUi *>(L_tab->getCurEditor());
        LOS_cmdPalette->showPalette(editor != nullptr ? static_cast<QWidget *>(editor) : L_tab->parentWidget());
    }



    /**
     * @brief showFindPopup
     * 打开查找 / 替换 / 跳转行弹窗
     * 弹窗打开期间按 Ctrl+F / Ctrl+H / Ctrl+G 
     * 会在三种弹窗之间互相切换
     * 切换以循环方式完成（关闭当前弹窗后继续打开目标弹窗）
     *
     * @param kind 初始弹窗类型
     */
    void LosFindPopupManager::showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind kind)
    {
        while (true)
        {
            if (kind == LosCommon::LosEditorTableUi_Constants::PopupKind::CommandPalette)
            {
                showCommandPalette();
                return;
            }
            if (kind == LosCommon::LosEditorTableUi_Constants::PopupKind::GotoLine)
            {
                auto widget = L_tab->getCurEditor();
                if (!widget)
                {
                    return;
                }
                auto *editor                               = qobject_cast<LosEditorUi *>(widget);
                int maxLines                               = editor->document()->blockCount();
                LosView::LosGotoLinePopupUi *contentWidget = new LosView::LosGotoLinePopupUi();
                LosView::LosFloatingPanelUi *dialog        = new LosView::LosFloatingPanelUi(contentWidget, true, L_tab);
                connect(contentWidget->getLineEdit(), &QLineEdit::returnPressed, dialog, &QDialog::accept);
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_findPopupSwitchRequested, contentWidget,
                        [this, dialog](bool with_replace)
                        {
                            L_pendingPopupKind = with_replace ? LosCommon::LosEditorTableUi_Constants::PopupKind::Replace
                                                              : LosCommon::LosEditorTableUi_Constants::PopupKind::Find;
                            dialog->reject();
                        });
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_commandPaletteOpenRequested, contentWidget,
                        [this, dialog]()
                        {
                            L_pendingPopupKind = LosCommon::LosEditorTableUi_Constants::PopupKind::CommandPalette;
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
                auto widget = L_tab->getCurEditor();
                if (!widget)
                {
                    return;
                }
                auto *edit                             = qobject_cast<LosEditorUi *>(widget);
                LosView::LosFindPopupUi *contentWidget = new LosView::LosFindPopupUi();
                LosView::LosFloatingPanelUi *dialog    = new LosView::LosFloatingPanelUi(contentWidget, true, L_tab);
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
                // 弹窗内切换查找/替换模式时，重新计算弹窗尺寸，避免替换按钮被挤压
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
                connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_commandPaletteOpenRequested, contentWidget,
                        [this, dialog]()
                        {
                            L_pendingPopupKind = LosCommon::LosEditorTableUi_Constants::PopupKind::CommandPalette;
                            dialog->reject();
                        });

                // 预填：选区文本 -> 上次搜索词 -> 光标处单词
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



} /* namespace LosView */
