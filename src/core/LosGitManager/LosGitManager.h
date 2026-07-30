// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QObject>
#include <git2.h>
#include <qmap.h>
#include <qset.h>
#include <qtmetamacros.h>

namespace LosCore
{
    class LosGitManager : public QObject
    {
        Q_OBJECT
      public: // instance
        static LosGitManager &instance();
        LosGitManager(const LosGitManager &)            = delete;
        LosGitManager &operator=(const LosGitManager &) = delete;

      public:
        QString lastErr() const;        // 上一个错误 所在的位置
        bool open(const QString &path); // 打开 .git 所在的文件夹
        QString curBranch() const;      // 当前的分支名字
        void status();
        unsigned int statusOfFile(const QString &absolute_file_path);
        bool folderHasBeenChanged(const QString &absolut_folder_path);

      private slots:
        void onInitGitProject(const QString &path);

      private: // init
        void initConnect();

      private: // construct
        explicit LosGitManager(QObject *parent = nullptr);
        ~LosGitManager();

      private: // params
        git_repository *L_repo = nullptr;
        QString L_lastErr;
        QMap<QString, unsigned int> L_filesStatus; // 绝对路径 -> 文件状态
        QSet<QString> L_dirtyFolders;
    };
} // namespace LosCore
