#include "LosGotoLinePopupUi.h"
#include "common/constants/ConstantsStr.h"


namespace LosView
{
    LosGotoLinePopupUi::LosGotoLinePopupUi(QWidget *parent) : QWidget(parent)
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        QLabel *label =
            new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LABEL_CONTENT);
        label->setStyleSheet(
            LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LABEL_STYLESHEET);
        L_lineEdit = new QLineEdit();
        L_lineEdit->setValidator(new QIntValidator(1, INT_MAX, L_lineEdit));
        L_lineEdit->setStyleSheet(
            LosCommon::LosFloatingPanelUi_Constants::LosGotoLinePopupUi_Constants::GOTOLINE_LINEEDIT_STYLESHEET);
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

} /* namespace LosView */
