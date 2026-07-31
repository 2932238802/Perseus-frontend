// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "view/LosFloatingPanelUi/LosFindPopupUi/LosFindPopupUi.h"
#include "common/constants/ConstantsStr/LosFloatingPanelUiStr.h"
#include "core/LosRouter/LosRouter.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


namespace LosView
{

    /**
     * @brief Construct a new Los Find Popup Ui:: Los Find Popup Ui object
     *
     * @param parent
     */
    LosFindPopupUi::LosFindPopupUi(QWidget *parent) : QWidget(parent)
    {
        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(6);

        /* ---------------- 第 1 行：查找 ---------------- */
        auto *findRow = new QHBoxLayout();
        findRow->setSpacing(8);
        auto *label = new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_CONTENT);
        label->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_STYLESHEET);
        L_lineEdit = new QLineEdit();
        L_lineEdit->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LINEEDIT_STYLESHEET);
        L_lineEdit->installEventFilter(this);
        L_matchLabel = new QLabel("0/0");
        L_matchLabel->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_MATCH_LABEL_STYLESHEET);
        findRow->addWidget(label);
        findRow->addWidget(L_lineEdit, 1);
        findRow->addWidget(L_matchLabel);
        mainLayout->addLayout(findRow);

        /* ---------------- 第 2 行：替换 ---------------- */
        L_replaceRow           = new QWidget(this);
        auto *replaceRowLayout = new QHBoxLayout(L_replaceRow);
        replaceRowLayout->setContentsMargins(0, 0, 0, 0);
        replaceRowLayout->setSpacing(8);
        auto *replaceLabel = new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::REPLACE_LABEL_CONTENT);
        replaceLabel->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_STYLESHEET);
        L_replaceEdit = new QLineEdit();
        L_replaceEdit->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::REPLACE_LINEEDIT_STYLESHEET);
        L_replaceEdit->installEventFilter(this);
        auto *replaceBtn    = new QPushButton("Replace");
        auto *replaceAllBtn = new QPushButton("Replace All");
        replaceBtn->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_BUTTON_STYLESHEET);
        replaceAllBtn->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_BUTTON_STYLESHEET);
        replaceRowLayout->addWidget(replaceLabel);
        replaceRowLayout->addWidget(L_replaceEdit, 1);
        replaceRowLayout->addWidget(replaceBtn);
        replaceRowLayout->addWidget(replaceAllBtn);
        mainLayout->addWidget(L_replaceRow);

        /* ---------------- 第 3 行：选项 ---------------- */
        auto *optionRow = new QHBoxLayout();
        optionRow->setSpacing(10);
        L_caseBox  = new QCheckBox("区分大小写");
        L_wordBox  = new QCheckBox("全词匹配");
        L_regexBox = new QCheckBox("正则");
        for (auto *box : {L_caseBox, L_wordBox, L_regexBox})
        {
            box->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_CHECKBOX_STYLESHEET);
            optionRow->addWidget(box);
            connect(box, &QCheckBox::toggled, this, [this]() { emit searchOptionsChanged(); });
        }
        optionRow->addStretch(1);
        mainLayout->addLayout(optionRow);

        /* ---------------- 按钮：替换 走 LosRouter ---------------- */
        connect(replaceBtn, &QPushButton::clicked, this, []() { emit LosCore::LosRouter::instance()._cmd_findReplaceRequested(); });
        connect(replaceAllBtn, &QPushButton::clicked, this, []() { emit LosCore::LosRouter::instance()._cmd_findReplaceAllRequested(); });
    }



    /**
     * @brief LosFindPopupUi 获取输入内容
     *
     * @return QString 输入内容
     */
    QString LosFindPopupUi::getInput() const
    {
        return L_lineEdit->text();
    }



    /**
     * @brief Get the Edit object
     *
     * @return QLineEdit
     */
    QLineEdit *LosFindPopupUi::getEdit() const
    {
        return L_lineEdit;
    }



    /**
     * @brief getReplaceInput 获取替换输入内容
     *
     * @return QString
     */
    QString LosFindPopupUi::getReplaceInput() const
    {
        return L_replaceEdit->text();
    }



    /**
     * @brief getReplaceEdit 获取替换输入框
     *
     * @return QLineEdit
     */
    QLineEdit *LosFindPopupUi::getReplaceEdit() const
    {
        return L_replaceEdit;
    }



    /**
     * @brief getFlags 根据勾选项生成查找标志
     *
     * @return QTextDocument::FindFlags
     */
    QTextDocument::FindFlags LosFindPopupUi::getFlags() const
    {
        QTextDocument::FindFlags flags;
        if (L_caseBox->isChecked())
        {
            flags |= QTextDocument::FindCaseSensitively;
        }
        if (L_wordBox->isChecked())
        {
            flags |= QTextDocument::FindWholeWords;
        }
        return flags;
    }



    /**
     * @brief isRegex 是否启用了正则模式
     *
     * @return bool
     */
    bool LosFindPopupUi::isRegex() const
    {
        return L_regexBox->isChecked();
    }



    /**
     * @brief setMatchInfo 设置匹配计数
     *
     * @param current 当前第几个匹配 (1 起)
     * @param total   匹配总数
     */
    void LosFindPopupUi::setMatchInfo(int current, int total)
    {
        if (total <= 0)
        {
            L_matchLabel->setText("0/0");
            return;
        }
        L_matchLabel->setText(QString("%1/%2").arg(current).arg(total));
    }



    /**
     * @brief setReplaceVisible 控制替换行的显示与隐藏
     *
     * @param visible
     */
    void LosFindPopupUi::setReplaceVisible(bool visible)
    {
        L_replaceRow->setVisible(visible);
    }



    /**
     * @brief keyPressEvent
     * 弹窗打开期间捕获 Ctrl+F / Ctrl+H / Ctrl+G / Ctrl+Shift+P：
     * - Ctrl+F / Ctrl+H 在当前弹窗上切换查找 / 替换模式
     * - Ctrl+G 切换到跳转行弹窗
     * - Ctrl+Shift+P 切换到指令面板
     * （LosShortcutManager 的快捷键为 WindowShortcut，弹窗激活时不会触发）
     */
    void LosFindPopupUi::keyPressEvent(QKeyEvent *event)
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            if (event->key() == Qt::Key_P && (event->modifiers() & Qt::ShiftModifier))
            {
                emit LosCore::LosRouter::instance()._cmd_commandPaletteOpenRequested();
                event->accept();
                return;
            }
            if (event->key() == Qt::Key_H)
            {
                setReplaceVisible(true);
                L_replaceEdit->setFocus();
                L_replaceEdit->selectAll();
                event->accept();
                emit resizeRequested();
                return;
            }
            if (event->key() == Qt::Key_F)
            {
                setReplaceVisible(false);
                L_lineEdit->setFocus();
                L_lineEdit->selectAll();
                event->accept();
                emit resizeRequested();
                return;
            }
            if (event->key() == Qt::Key_G)
            {
                emit LosCore::LosRouter::instance()._cmd_gotoLinePopupSwitchRequested();
                event->accept();
                return;
            }
        }
        QWidget::keyPressEvent(event);
    }



    /**
     * @brief eventFilter
     * 拦截输入框回车，区分 Shift+Enter 与 Enter
     * 经 LosRouter 信号总线发出，由编辑器标签页处理
     */
    bool LosFindPopupUi::eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::KeyPress)
        {
            auto *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                if (obj == L_lineEdit)
                {
                    if (keyEvent->modifiers() & Qt::ShiftModifier)
                    {
                        emit LosCore::LosRouter::instance()._cmd_findPreviousRequested();
                    }
                    else
                    {
                        emit LosCore::LosRouter::instance()._cmd_findNextRequested();
                    }
                    return true;
                }
                if (obj == L_replaceEdit)
                {
                    emit LosCore::LosRouter::instance()._cmd_findReplaceRequested();
                    return true;
                }
            }
            // 输入框焦点下也捕获 Ctrl+Shift+P（keyPressEvent 可能收不到）
            if (keyEvent->key() == Qt::Key_P && (keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->modifiers() & Qt::ShiftModifier))
            {
                emit LosCore::LosRouter::instance()._cmd_commandPaletteOpenRequested();
                return true;
            }
        }
        return QWidget::eventFilter(obj, event);
    }



} // namespace LosView
