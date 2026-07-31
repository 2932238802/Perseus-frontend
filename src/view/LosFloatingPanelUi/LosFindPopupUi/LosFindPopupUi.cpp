// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "view/LosFloatingPanelUi/LosFindPopupUi/LosFindPopupUi.h"
#include "common/constants/ConstantsStr/LosFloatingPanelUiStr.h"
#include "core/LosRouter/LosRouter.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>


namespace LosView
{

    /**
     * @brief Construct a new Los Find Popup Ui:: Los Find Popup Ui object
     *
     * @param parent
     */
    LosFindPopupUi::LosFindPopupUi(QWidget *parent) : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(8);
        QLabel *label = new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_CONTENT);
        label->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_STYLESHEET);
        L_lineEdit = new QLineEdit();
        L_lineEdit->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LINEEDIT_STYLESHEET);
        L_lineEdit->installEventFilter(this);
        L_matchLabel = new QLabel("0/0");
        L_matchLabel->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_MATCH_LABEL_STYLESHEET);
        layout->addWidget(label);
        layout->addWidget(L_lineEdit, 1);
        layout->addWidget(L_matchLabel);
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
     * @brief eventFilter
     * 拦截输入框回车，区分 Shift+Enter 与 Enter
     * 经 LosRouter 信号总线发出，由编辑器标签页处理
     */
    bool LosFindPopupUi::eventFilter(QObject *obj, QEvent *event)
    {
        if (obj == L_lineEdit && event->type() == QEvent::KeyPress)
        {
            auto *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
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
        }
        return QWidget::eventFilter(obj, event);
    }



} // namespace LosView
