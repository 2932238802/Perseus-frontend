// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "models/LosFilePath/LosFilePath.h"
#include <QList>
#include <QString>
#include <functional>

namespace LosModel
{
    class LosFileNode
    {
      public:
        ~LosFileNode();

      public: // tool
        [[nodiscard]] static LosFileNode *create(const QString &filePath, LosFileNode *parent);

      public: // set
        void appendChild(LosFileNode *node);
        void clear();
        void setParentNode(LosFileNode *node);
        static void build(LosFileNode *root, const QString &cur, std::function<void()> on_finished);
        static void buildImpl(LosFileNode *, const QString &);

      public: // get
        int getChildCount() const;
        LosFileNode *getChild(int index) const;
        LosFileNode *getParent() const;
        int row() const;
        LosModel::LosFilePath &getFile();
        LosCommon::LOS_ENUM_FileType getFileType() const;

      private:
        explicit LosFileNode(const QString &filePath, LosFileNode *parent = nullptr);

      private:
        LosFilePath LOS_filePath;
        QList<LosFileNode *> LOS_childrens;
        LosFileNode *LOS_parent = nullptr;
    };
}; /* namespace LosModel */