#include "LosToolMissUi.h"


namespace LosView
{

    LosToolMissUi::LosToolMissUi(const LosCommon::LosToolChain_Constants::ToolChainConfig &config, QWidget *parent)
        : QDialog{parent}
    {
        initStyle(config);
    }



    /*
     * - 设置 样式
     */
    void LosToolMissUi::initStyle(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
    {
        setWindowTitle("Tool Missing");
        setMinimumWidth(400);
        auto *layout = new QVBoxLayout(this);
        auto *label  = new QLabel(QString("%1 not find!").arg(config.L_name));
        QFont font   = label->font();
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        layout->addWidget(label);

        auto *descLabel = new QLabel(QString("Perseus needs <b>%1</b> to compile and run your code.<br><br>"
                                             "Expected executable: <code>%2</code><br><br>"
                                             "You can:")
                                         .arg(config.L_name)
                                         .arg(config.L_exeName.join(", ")));
        descLabel->setWordWrap(true);
        layout->addWidget(descLabel);
        layout->addSpacing(10);

        /*
         * 增加 按钮 这个 按钮 是下载的
         */
        auto *btnLayout   = new QHBoxLayout();
        auto *downloadBtn = new QPushButton("Open Download Page");
        connect(downloadBtn, &QPushButton::clicked, this,
                [this, config]()
                {
                    if (!config.L_downUrl.isEmpty())
                    {
                        QDesktopServices::openUrl(QUrl(config.L_downUrl));
                    }
                    accept();
                });
        btnLayout->addWidget(downloadBtn);

        auto *configBtn = new QPushButton("Manual Setup");
        connect(configBtn, &QPushButton::clicked, this,
                [this, config]()
                {
                    emit LosCore::LosRouter::instance()._cmd_manuallySet(config);
                    accept();
                });
        btnLayout->addWidget(configBtn);

        auto *cancelBtn = new QPushButton("Cancel");
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        auto *hintLabel =
            new QLabel("<small>after installation, restart Perseus or manually set the path in settings! </small>");
        hintLabel->setWordWrap(true);

        /*
         * 灰色先看看
         */
        hintLabel->setStyleSheet("color: gray;");
        layout->addWidget(hintLabel);

        setStyleSheet(LosStyle::LosToolMissUI_getStyle());
    }

} /* namespace LosView */