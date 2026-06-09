// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentManager.h"
#include "core/LosRouter/LosRouter.h"

namespace LosCore
{
    /**
     * @brief Construct a new Los Agent Manager:: Los Agent Manager object
     * - 单例, 构造时建立信号连接
     */
    LosAgentManager::LosAgentManager(QObject *parent) : QObject(parent)
    {
        initConnect();
    }



    /**
     * @brief initConnect
     * - 监听 UI 发来的用户消息
     */
    void LosAgentManager::initConnect()
    {
        auto &router = LosRouter::instance();
        connect(&router, &LosRouter::_cmd_agent_sendMessage, this, [this](const QString &message) { this->handleMessage(message); });
    }



    /**
     * @brief handleMessage
     * - 处理用户消息
     * - 第一阶段: 本地假回复
     * - 后续: 改为调用 LosAgentClient 请求后端 (拿 user_id 对应的 ai-key, 组织上下文)
     *
     * @param message 用户发来的消息
     */
    void LosAgentManager::handleMessage(const QString &message)
    {
        const QString reply = QStringLiteral("收到：") + message;
        emit LosRouter::instance()._cmd_agent_reply(reply);
    }

} /* namespace LosCore */
