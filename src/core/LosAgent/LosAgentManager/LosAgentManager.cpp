// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentManager.h"
#include "core/LosNet/LosNet.h"
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
        connect(&router, &LosRouter::_cmd_agent_sendMessage, this,
                [this](const QString &message, const QString &provider, const QString &model) { this->handleMessage(message, provider, model); });
    }



    /**
     * @brief handleMessage
     * @param message  用户发来的消息
     * @param provider 当前选中厂商
     * @param model    当前选中模型
     */
    void LosAgentManager::handleMessage(const QString &message, const QString &provider, const QString &model)
    {
        LosNet::instance().requestAgentChatStream(message, provider, model);
    }

} /* namespace LosCore */
