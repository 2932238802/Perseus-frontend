// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "view/LosCompleterUi/LosCompleterUi.h"

namespace LosView
{
    /**
     * @brief Construct a new Los Completer Ui:: Los Completer Ui object
     *
     * @param parent
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



    /**
     * @brief updateCompletionList 更新补全的内容
     *
     * @param list
     */
    void LosCompleterUi::updateCompletionList(const QStringList &list)
    {
        if (L_model->stringList() == list)
        {
            return;
        }
        L_model->setStringList(list);
    }



    /**
     * @brief initStyle
     * - 初始化样式
     */
    void LosCompleterUi::initStyle()
    {
        auto popup = this->popup();
        popup->setStyleSheet(LosStyle::LosCompleterUi_getStyle());
        popup->setFrameShape(QFrame::NoFrame);
        popup->setAttribute(Qt::WA_TranslucentBackground, false);
        popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        popup->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        popup->setTextElideMode(Qt::ElideRight);
    }

} /* namespace LosView */