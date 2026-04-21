
#pragma once
#include <QString>
#include <QStringList>

namespace LosCommon
{


    /*
     * - 保留 会话 的固定字段
     */
    namespace LosSession_Constants
    {
        struct Config
        {
            QStringList L_curFilePaths{};
            QString L_curProDir     = "";
            QString L_curActiveFile = "";
        };

    } /* namespace LosSession_Constants */


} // namespace LosCommon