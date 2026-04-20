
#include "view/LosFloatingPanelUi/LosFindPopupUi/LosFindPopupUi.h"


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
        QLabel *label =
            new QLabel(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_CONTENT);
        label->setStyleSheet(LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LABEL_STYLESHEET);
        L_lineEdit = new QLineEdit();
        L_lineEdit->setValidator(new QIntValidator(1, INT_MAX, L_lineEdit));
        L_lineEdit->setStyleSheet(
            LosCommon::LosFloatingPanelUi_Constants::LosFindPopupUi_Constants::FIND_LINEEDIT_STYLESHEET);
        layout->addWidget(label);
        layout->addWidget(L_lineEdit);
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



} // namespace LosView