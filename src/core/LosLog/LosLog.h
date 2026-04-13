#pragma once
#include <QDateTime>
#include <QObject>
#include <QString>

namespace LosCore
{

    class LosLog : public QObject
    {
        enum LogType
        {
            INFO,
            WARNING,
            ERROR,
            SUC,
        };

        Q_OBJECT
      public: /* tool */
        static LosLog &instance();
        void inf(const QString &msg, const QString &comp = "System");
        void war(const QString &msg, const QString &comp = "System");
        void err(const QString &msg, const QString &comp = "System");
        void suc(const QString &msg, const QString &comp = "System");

      private: /* tool */
        void log(LogType type, const QString &msg, const QString &comp = "System");
        QString getColor(LogType);
        QString getPrefix(LogType);

      signals:
        void _sendLog(const QString &log);

      private: /* construct */
        explicit LosLog(QObject *parent = nullptr);
        ~LosLog();
    };
} /* namespace LosCore */

#define INF(msg, cmp) LosCore::LosLog::instance().inf(msg, cmp);
#define WAR(msg, cmp) LosCore::LosLog::instance().war(msg, cmp);
#define ERR(msg, cmp) LosCore::LosLog::instance().err(msg, cmp);
#define SUC(msg, cmp) LosCore::LosLog::instance().suc(msg, cmp);
