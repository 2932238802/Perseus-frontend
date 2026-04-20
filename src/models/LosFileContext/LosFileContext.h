

#pragma once

#include "models/LosFilePath/LosFilePath.h"
#include <QFile>
#include <QFileInfo>
#include <QString>

namespace LosModel
{
    class LosFileContext
    {
      public:
        std::optional<QString> load(const QString &file_path);
        bool save(const QString &content, const QString &file_path);
        bool isLoaded() const;
        bool isEmpty() const;
        const QString &getContent() const;
        [[nodiscard]] static LosFileContext *create();

      public:
        ~LosFileContext() = default;
        LosFileContext(const QString &file_path);
        LosFileContext(const LosModel::LosFilePath &path);
        LosFileContext();

      private:
        QString L_content = "";
        bool L_isLoaded   = false;
    };
}; /* namespace LosModel */