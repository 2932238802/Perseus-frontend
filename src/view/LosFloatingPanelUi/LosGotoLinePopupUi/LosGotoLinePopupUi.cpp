// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosGotoLinePopupUi.h"
#include "common/constants/ConstantsStr/LosFloatingPanelUiStr.h"
#include "core/LosRouter/LosRouter.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>


namespace LosView
{
    LosGotoLinePopupUi::LosGotoLinePopupUi(QWidget *parent) : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        QLabel *label = new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LABEL_CONTENT);
        label->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LABEL_STYLESHEET);
        L_lineEdit = new QLineEdit();
        L_lineEdit->setValidator(new QIntValidator(1, INT_MAX, L_lineEdit));
        L_lineEdit->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LINEEDIT_STYLESHEET);
        layout->addWidget(label);
        layout->addWidget(L_lineEdit);
    }



    /*
     * - 获取 行号
     */
    int LosGotoLinePopupUi::getLineNumber() const
    {
        return L_lineEdit->text().toInt();
    }



    /*
     * - 获取 输入框
     */
    QLineEdit *LosGotoLinePopupUi::getLineEdit() const
    {
        return L_lineEdit;
    }



    /**
     * @brief keyPressEvent
     * 弹窗打开期间捕获 Ctrl+F / Ctrl+H，切换到查找 / 替换弹窗
     */
    void LosGotoLinePopupUi::keyPressEvent(QKeyEvent *event)
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            if (event->key() == Qt::Key_F)
            {
                emit LosCore::LosRouter::instance()._cmd_findPopupSwitchRequested(false);
                event->accept();
                return;
            }
            if (event->key() == Qt::Key_H)
            {
                emit LosCore::LosRouter::instance()._cmd_findPopupSwitchRequested(true);
                event->accept();
                return;
            }
        }
        QWidget::keyPressEvent(event);
    }

} /* namespace LosView */
