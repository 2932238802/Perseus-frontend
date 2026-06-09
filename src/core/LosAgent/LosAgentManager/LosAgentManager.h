// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QObject>

namespace LosCore
{
    class LosAgentManager : public QObject
    {
        Q_OBJECT
      public:
        static LosAgentManager &instance()
        {
            static LosAgentManager _instance;
            return _instance;
        }
        LosAgentManager(const LosAgentManager &)            = delete;
        LosAgentManager &operator=(const LosAgentManager &) = delete;

      private:
        explicit LosAgentManager(QObject *parent = nullptr);
        ~LosAgentManager() = default;

        void initConnect();

      private: // tool
        void handleMessage(const QString &message);
    };

} /* namespace LosCore */
