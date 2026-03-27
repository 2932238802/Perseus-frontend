
#include "view/LosCompleterUi/LosCompleterUi.h"

namespace LosView
{
/**
const
*/
LosCompleterUi::LosCompleterUi(QObject *parent) : QCompleter{parent}
{
    L_model = new QStringListModel(this);
    this->setModel(L_model);
    setMaxVisibleItems(LosCommon::MAX_VISIBLE_COMPLETION);
    setCompletionMode(QCompleter::PopupCompletion);
    // 大小写不敏感
    setCaseSensitivity(Qt::CaseInsensitive);
    initStyle();
}
LosCompleterUi::~LosCompleterUi() {}



/**
set
*/
void LosCompleterUi::updateCompletionList(const QStringList &list)
{
    L_model->setStringList(list);
}



/**
init
*/
void LosCompleterUi::initStyle()
{
    auto popup = this->popup();
    popup->setStyleSheet(LosStyle::LosCompleterUi_getStyle());
}

} // namespace LosView