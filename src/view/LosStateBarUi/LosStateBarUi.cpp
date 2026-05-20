#include "LosStateBarUi.h"
#include <QHBoxLayout>
#include <QSpacerItem>

namespace LosView
{
    LosStateBarUi::LosStateBarUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
    }

    void LosStateBarUi::initStyle()
    {
        setMinimumHeight(24);
        setMaximumHeight(24);
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(8, 0, 8, 0);
        layout->setSpacing(4);
        L_messageLabel = new QLabel("Ready", this);
        L_messageLabel->setObjectName("statusbar_label");
        layout->addWidget(L_messageLabel);
        layout->addStretch();
        L_langLabel = new QLabel("UnKnow File", this);
        L_langLabel->setObjectName("statusbar_lang_label");
        layout->addWidget(L_langLabel);
    }

} // namespace LosView
