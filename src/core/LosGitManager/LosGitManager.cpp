// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "core/LosGitManager/LosGitManager.h"
#include "core/LosRouter/LosRouter.h"
#include "git2/errors.h"
#include "git2/refs.h"
#include "git2/repository.h"
#include "git2/status.h"
#include "git2/types.h"
#include <cstddef>
#include <git2.h>
#include <qfileinfo.h>

namespace LosCore
{
    /**
     * @brief 单例模式
     *
     * @return LosGitManager&
     */
    LosGitManager &LosGitManager::instance()
    {
        static LosGitManager m;
        return m;
    }


    /**
     * @brief Construct a new LosGitManager object
     * - 构造时初始化 libgit2 全局状态 引用计数 +1
     * @param parent
     */
    LosGitManager::LosGitManager(QObject *parent) : QObject(parent)
    {
        int err = git_libgit2_init();
        initConnect();
    }


    /**
     * @brief Destroy the LosGitManager object
     * - 析构时关闭 libgit2（引用计数 -1归零时真正释放全局资源
     */
    LosGitManager::~LosGitManager()
    {
        git_repository_free(L_repo);
        git_libgit2_shutdown();
    }



    /**
     * @brief
     *
     * @return QString
     */
    QString LosGitManager::lastErr() const
    {
        const git_error *e = git_error_last();
        return e ? QString::fromUtf8(e->message) : QStringLiteral("unkown git err");
    }



    /**
     * @brief 打开当前的项目路径
     *
     * @param path
     * @return true
     * @return false
     */
    bool LosGitManager::open(const QString &path)
    {
        if (L_repo != nullptr)
        {
            git_repository_free(L_repo);
            L_repo = nullptr;
        }
        int err = git_repository_open(&L_repo, path.toUtf8().constData());
        if (err != 0)
        {
            L_lastErr = lastErr();
            L_repo    = nullptr;
            return false;
        }
        // 成功了
        L_lastErr.clear();
        return true;
    }



    /**
     * @brief 获得当前分支名
     * @return QString
     */
    QString LosGitManager::curBranch() const
    {
        if (L_repo == nullptr)
        {
            return QString{};
        }
        git_reference *head = nullptr;
        int err             = git_repository_head(&head, L_repo);
        if (err != 0)
        {
            return QString{};
        }
        QString res;
        const char *name = git_reference_shorthand(head); // git_reference_shorthand 名字更短
        if (name != nullptr)
        {
            res = QString::fromUtf8(name);
        }
        git_reference_free(head);
        head = nullptr;
        return res;
    }



    /**
     * @brief status 获取所有的文件的状态
     */
    void LosGitManager::status()
    {
        if (L_repo == nullptr)
            return;
        const char *workDir = git_repository_workdir(L_repo);
        L_filesStatus.clear();
        if (workDir == nullptr)
            return;
        QString baseDir         = QString::fromUtf8(workDir);
        git_status_options opts = GIT_STATUS_OPTIONS_INIT;
        opts.show               = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        opts.flags              = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
        git_status_list *list   = nullptr; // 隐藏结构体
        int err                 = git_status_list_new(&list, L_repo, &opts);
        if (err != 0)
        {
            L_lastErr = lastErr();
            return;
        };
        size_t count = git_status_list_entrycount(list);
        for (size_t i = 0; i < count; i++)
        {
            const git_status_entry *e = git_status_byindex(list, i);
            if (e == nullptr)
                continue;
            const char *relPath = nullptr;
            if (e->head_to_index != nullptr)
            {
                relPath = e->head_to_index->old_file.path;
            }
            else if (e->index_to_workdir)
            {
                relPath = e->index_to_workdir->old_file.path;
            }
            if (relPath == nullptr)
            {
                continue;
            }
            QString absPath = baseDir + relPath;
            L_filesStatus.insert(absPath, e->status);
            QString fatherfolder = QFileInfo(absPath).absolutePath(); // 文件夹的路径
            while (fatherfolder.startsWith(baseDir) && fatherfolder.size() >= baseDir.size())
            {
                L_dirtyFolders.insert(fatherfolder);
                QString up = QFileInfo(fatherfolder).absolutePath();
                if (up == fatherfolder)
                    break;
                fatherfolder = up;
            }
        }
        git_status_list_free(list);
        emit LosRouter::instance()._cmd_gitStatusUpdated();
    }



    /**
     * @brief
     *
     * @param absolute_file_path
     * @return unsigned int
     */
    unsigned int LosGitManager::statusOfFile(const QString &absolute_file_path)
    {
        return L_filesStatus.value(absolute_file_path, GIT_STATUS_CURRENT);
    }



    /**
     * @brief folderHasBeenChanged 返回文件夹是否被修改了
     *
     * @param absolut_folder_path
     * @return true
     * @return false
     */
    bool LosGitManager::folderHasBeenChanged(const QString &absolut_folder_path)
    {
        return L_dirtyFolders.contains(absolut_folder_path);
    }



    /**
     * @brief
     *
     * @param path
     */
    void LosGitManager::onInitGitProject(const QString &path)
    {
        if (open(path))
        {
            emit LosRouter::instance()._cmd_getBranchName(curBranch());
            status();
        }
        else
        {
            emit LosRouter::instance()._cmd_getBranchName("no git");
        }
    }



    /**
     * @brief
     *
     */
    void LosGitManager::initConnect()
    {
        auto &router = LosRouter::instance();
        connect(&router, &LosRouter::_cmd_initGitProject, this, &LosGitManager::onInitGitProject);
        connect(&router, &LosRouter::_cmd_fileDirty, this,
                [this](const QString &, bool dirty)
                {
                    if (!dirty)
                    {
                        status();
                    }
                });
        connect(&router, &LosRouter::_cmd_fileSystemChanged, this, [this]() { status(); });
    }
} // namespace LosCore
