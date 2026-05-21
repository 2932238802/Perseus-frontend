#include "LosStateBarUi.h"
#include "common/util/CheckLang.h"
#include "common/util/GetLangId.h"
#include "core/LosEncodingDetector/LosEncodingDetector.h"
#include "core/LosRouter/LosRouter.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <qlabel.h>

namespace LosView
{
    /**
     * @brief Construct a new Los State Bar Ui:: Los State Bar Ui object
     *
     * @param parent
     */
    LosStateBarUi::LosStateBarUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
        initConnect();
    }



    /**
     * @brief setCodingBarLabel
     *
     * @param text
     */
    void LosStateBarUi::setCodingBarLabel(const QString &text)
    {
        if (!L_codingLabel)
        {
            return;
        }
        L_codingLabel->setText(text);
    }



    /**
     * @brief setLangLabel
     *
     * @param text
     */
    void LosStateBarUi::setLangLabel(const QString &text)
    {
        if (!L_langLabel)
        {
            return;
        }
        L_langLabel->setText(text);
    }



    /**
     * @brief initStyle
     * 初始化样式
     */
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
        L_codingLabel = new QLabel("UTF-8", this);
        L_codingLabel->setObjectName("statusbar_coding_label");
        L_langLabel = new QLabel("UnKnow File", this);
        L_langLabel->setObjectName("statusbar_lang_label");
        layout->addWidget(L_codingLabel);
        layout->addWidget(L_langLabel);
    }



    /**
     * @brief initConnect
     * 连接
     */
    void LosStateBarUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_fileChanged, this,
                [this](const QString &absolute_file_path)
                {
                    QString text = LosCore::LosEncodingDetector::detectFromFile(absolute_file_path);
                    setCodingBarLabel(text);
                    // 当前先 简单的判断一下 文件后缀名
                    setLangLabel(LosCommon::getLangId(LosCommon::CheckLang(absolute_file_path)));
                });
    }

} // namespace LosView
