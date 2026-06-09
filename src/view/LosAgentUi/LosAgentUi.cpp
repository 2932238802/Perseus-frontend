// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentUi.h"
#include "./ui_LosAgentUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/style/LosAgent_style.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>

namespace LosView
{
    /**
     * @brief 构造函数
     * - 构建控件 -> 着色 -> 连接信号
     */
    LosAgentUi::LosAgentUi(QWidget *parent) : QWidget(parent), ui(new Ui::LosAgentUi)
    {
        initUi();
        initStyle();
        initConnect();
    }
    LosAgentUi::~LosAgentUi()
    {
        delete ui;
    }



    /**
     * @brief   装载 .ui 表单
     */
    void LosAgentUi::initUi()
    {
        ui->setupUi(this);
    }



    /**
     * @brief initStyle
     * - 用当前主题着色
     */
    void LosAgentUi::initStyle()
    {
        applyTheme(LosCore::LosThemeManager::instance().currentTheme());
    }



    /**
     * @brief applyTheme
     * - 主题换肤回调 (与 LosAuthUi 约定一致)
     * - 把 LosAgent_style.h 模板里的 @token@ 替换为当前主题色后应用
     */
    void LosAgentUi::applyTheme(const QString &themeName)
    {
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::losAgent_getStyleTemplate(), themeName);
        setStyleSheet(qss);
    }



    /**
     * @brief initConnect
     * - 发送按钮 / 回车提交 / Agent 回包 / 主题换肤
     */
    void LosAgentUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(ui->send_btn, &QPushButton::clicked, this, &LosAgentUi::onSendClicked);
        connect(ui->input_edit, &QLineEdit::returnPressed, this, &LosAgentUi::onSendClicked);
        connect(&router, &LosCore::LosRouter::_cmd_agent_reply, this, &LosAgentUi::onAgentReply);
        connect(&router, &LosCore::LosRouter::_cmd_agent_error, this, &LosAgentUi::onAgentError);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
    }



    /**
     * @brief onSendClicked
     * - 立刻显示用户气泡 -> 把消息抛给 core (LosAgentManager)
     */
    void LosAgentUi::onSendClicked()
    {
        const QString text = ui->input_edit->text().trimmed();
        if (text.isEmpty())
            return;
        addBubble(Role::User, text);
        emit LosCore::LosRouter::instance()._cmd_agent_sendMessage(text);
        ui->input_edit->clear();
    }



    /**
     * @brief onAgentReply
     */
    void LosAgentUi::onAgentReply(const QString &message)
    {
        addBubble(Role::Agent, message);
    }



    /**
     * @brief 
     * 
     */
    void LosAgentUi::onAgentError(const QString &message)
    {
        addBubble(Role::Agent, message);
    }



    /**
     * @brief addBubble
     * - 向消息列表追加一个气泡
     * - User 靠右, Agent 靠左; 配色由 objectName 在 LosAgent_style.h 控制
     *
     * @param role     消息角色
     * @param content  消息内容
     */
    void LosAgentUi::addBubble(Role role, const QString &content)
    {
        const bool isUser    = (role == Role::User);
        const int viewW      = ui->chat_view->viewport()->width();
        const int maxBubbleW = qMax(120, static_cast<int>(viewW * 0.75));
        const int padH = 6; 
        const int padV = 4; 
        QLabel *bubble = new QLabel(content);
        bubble->setObjectName(isUser ? QStringLiteral("agentBubbleUser") : QStringLiteral("agentBubbleAgent"));
        bubble->setWordWrap(true);
        bubble->setTextInteractionFlags(Qt::TextSelectableByMouse);
        bubble->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        bubble->setContentsMargins(padH, padV, padH, padV);
        bubble->setMaximumWidth(maxBubbleW);
        const int textW = maxBubbleW - padH * 2;
        int textH       = bubble->heightForWidth(textW);
        if (textH <= 0)
            textH = bubble->fontMetrics().height();
        const int bubbleH = textH + padV * 2;
        QWidget *holder  = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(holder);
        lay->setContentsMargins(8, 4, 8, 4);
        if (isUser)
        {
            lay->addStretch(1);
            lay->addWidget(bubble);
        }
        else
        {
            lay->addWidget(bubble);
            lay->addStretch(1);
        }
        const int itemH       = bubbleH + 8 + 6;
        QListWidgetItem *item = new QListWidgetItem(ui->chat_view);
        item->setSizeHint(QSize(viewW, itemH));
        ui->chat_view->setItemWidget(item, holder);
        ui->chat_view->scrollToBottom();
    }
} /* namespace LosView */
