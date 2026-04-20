
#include "view/LosCompleterUi/LosCompleterUi.h"

namespace LosView
{
    /*
     * const
     */
    LosCompleterUi::LosCompleterUi(QObject *parent) : QCompleter{parent}
    {
        L_model = new QStringListModel(this);
        this->setModel(L_model);
        setMaxVisibleItems(LosCommon::MAX_VISIBLE_COMPLETION);
        setCompletionMode(QCompleter::PopupCompletion);
        setCaseSensitivity(Qt::CaseInsensitive);
        initStyle();
    }
    LosCompleterUi::~LosCompleterUi() {}



    /*
     * set
     * - 重复的 就不要 在显示了
     */
    void LosCompleterUi::updateCompletionList(const QStringList &list)
    {
        if (L_model->stringList() == list)
        {
            return;
        }
        L_model->setStringList(list);
    }



    /*
     * init
     */
    void LosCompleterUi::initStyle()
    {
        auto popup = this->popup();
        popup->setStyleSheet(LosStyle::LosCompleterUi_getStyle());
    }

} /* namespace LosView */