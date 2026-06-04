// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "core/LosGitManager/LosGitManager.h"
#include "git2/global.h"
#include <qobject.h>

namespace LosCore
{

    LosGitManager &LosGitManager::instance()
    {
        static LosGitManager m;
        return m;
    }
    LosGitManager::~LosGitManager()
    {
        int err = git_libgit2_shutdown();
    }
    LosGitManager::LosGitManager(QObject *parent) : QObject(parent)
    {
        int err = git_libgit2_init();
    }

} // namespace LosCore