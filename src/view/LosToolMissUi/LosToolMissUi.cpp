#include "LosToolMissUi.h"


namespace LosView
{
    LosToolMissUi::LosToolMissUi(const LosCommon::LosToolChain_Constants::ToolChainConfig &config, QWidget *parent)
        : QDialog{parent}
    {
        initStyle(config);
    }

    void LosToolMissUi::initStyle(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
    {
        setWindowTitle("Tool Missing");
        setMinimumWidth(450);
        auto *layout = new QVBoxLayout(this);
        
        auto *label  = new QLabel(QString("%1 not found!").arg(config.L_name));
        QFont font   = label->font();
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        layout->addWidget(label);

        auto *descLabel = new QLabel(QString("Perseus needs <b>%1</b> to compile and run your code.<br><br>"
                                             "Expected executable: <code>%2</code><br><br>"
                                             "How would you like to resolve this?")
                                         .arg(config.L_name)
                                         .arg(config.L_exeName.join(", ")));
        descLabel->setWordWrap(true);
        layout->addWidget(descLabel);
        layout->addSpacing(15);

        auto *autoInstallBtn = new QPushButton("Auto Install (Run in background)");
        autoInstallBtn->setObjectName("primaryBtn");
        autoInstallBtn->setCursor(Qt::PointingHandCursor); 
        connect(autoInstallBtn, &QPushButton::clicked, this, [this, config]() {
            emit LosCore::LosRouter::instance()._cmd_autoInstallTool(config);
            accept(); 
        });
        layout->addWidget(autoInstallBtn);

        layout->addSpacing(5);

        auto *secondaryBtnLayout = new QHBoxLayout();
        
        auto *downloadBtn = new QPushButton("Open Website");
        downloadBtn->setCursor(Qt::PointingHandCursor);
        connect(downloadBtn, &QPushButton::clicked, this, [this, config]() {
            if (!config.L_downUrl.isEmpty()) {
                QDesktopServices::openUrl(QUrl(config.L_downUrl));
            }
            accept();
        });
        secondaryBtnLayout->addWidget(downloadBtn);

        auto *configBtn = new QPushButton("Manual Setup");
        configBtn->setCursor(Qt::PointingHandCursor);
        connect(configBtn, &QPushButton::clicked, this, [this, config]() {
            emit LosCore::LosRouter::instance()._cmd_manuallySet(config);
            accept();
        });
        secondaryBtnLayout->addWidget(configBtn);

        auto *cancelBtn = new QPushButton("Cancel");
        cancelBtn->setCursor(Qt::PointingHandCursor);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        secondaryBtnLayout->addWidget(cancelBtn);

        layout->addLayout(secondaryBtnLayout);

        layout->addSpacing(10);
        auto *hintLabel = new QLabel("<small>After installation, restart Perseus or manually set the path in settings.</small>");
        hintLabel->setWordWrap(true);
        hintLabel->setStyleSheet("color: #888888;"); // 稍微调亮一点的灰色，#888888 更好看
        layout->addWidget(hintLabel);

        setStyleSheet(LosStyle::LosToolMissUI_getStyle());
    }
} /* namespace LosView */
