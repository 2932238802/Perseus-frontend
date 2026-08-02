// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"

#include <QMap>
#include <QObject>

namespace LosCore
{
    class LosLspClient;
    class LosLspManager : public QObject
    {
        Q_OBJECT
      public:
        explicit LosLspManager(QObject *parent = nullptr);
        ~LosLspManager() override;

      private: /* init */
        void initConnect();

      private: /* tool */
        void openFile(const QString &file_path, const QString &file_context);
        void changeFile(const QString &file_path, const QString &file_context);
        void requestCompletion(const QString &file_path, int line, int col);
        void requestHover(const QString &file_path, int line, int col);
        void toDefineRequest(int line, int col, const QString &file_path);
        void didChangeWatchedFiles(const QString &file_path, int type);

      private: // get
        LosLspClient *getClient(const QString &);

      private slots:
        void onLspReady(LosCommon::LosToolChain_Constants::LosTool, const QString &, const QStringList &);
        void onFileRenamed(const QString &oldPath, const QString &newPath);
        void onSemantic(const QString &);

      private:
        // 一个语言对应一个解释器
        // LosTool 指定工具
        QMap<LosCommon::LosToolChain_Constants::LosTool, LosLspClient *> LOS_clients;
    };

} /* namespace LosCore */
