// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QObject>
#include <qobject.h>

namespace LosCore
{
    class LosGitManager : public QObject
    {
      public: // instance
        static LosGitManager &instance();
        LosGitManager(const LosGitManager &)            = delete;
        LosGitManager &operator=(const LosGitManager &) = delete;

      private: // construct
        explicit LosGitManager(QObject *parent = nullptr);
        ~LosGitManager();
    };
} // namespace LosCore