
#include "core/LosLog/LosLog.h"
namespace LosCore
{

    LosLog &LosLog::instance()
    {
        static LosLog log;
        return log;
    }

    void LosLog::inf(const QString &msg, const QString &comp)
    {
        log(LogType::INFO, msg, comp);
    }
    void LosLog::war(const QString &msg, const QString &comp)
    {
        log(LogType::WARNING, msg, comp);
    }
    void LosLog::err(const QString &msg, const QString &comp)
    {
        log(LogType::ERROR, msg, comp);
    }
    void LosLog::suc(const QString &msg, const QString &comp)
    {
        log(LogType::SUC, msg, comp);
    }

    void LosLog::log(LogType type, const QString &msg, const QString &comp)
    {
        QString timeStr   = QDateTime::currentDateTime().toString("HH:mm:ss");
        QString color     = getColor(type);
        QString prefix    = getPrefix(type);
        QString msgToHtml = msg.toHtmlEscaped().replace("\n", "<br>");
        QString html      = QString("<span style='color:gray;'>[%1]</span> "
                                         "<span style='color:#569CD6;'>[%2]</span> "
                                         "<span style='color:%3;'><b>%4</b> %5</span>")
                           .arg(timeStr)
                           .arg(comp)
                           .arg(color)
                           .arg(prefix)
                           .arg(msgToHtml);

        emit _sendLog(html);
    }


    /*
     * - 获取对应的 水平的颜色
     * -
     */
    QString LosLog::getColor(LogType level)
    {
        switch (level)
        {
        case LogType::INFO:
            return "#D4D4D4";
        case LogType::WARNING:
            return "#DCDCAA";
        case LogType::ERROR:
            return "#F44747";
        case LogType::SUC:
            return "#B5CEA8";
        default:
            return "#FFFFFF";
        }
    }


    /*
     * - 获取 打印的 前后缀
     */
    QString LosLog::getPrefix(LogType level)
    {
        switch (level)
        {
        case LogType::INFO:
            return "[INFO]";
        case LogType::WARNING:
            return "[WARN]";
        case LogType::ERROR:
            return "[ERROR]";
        case LogType::SUC:
            return "[SUCC]";
        default:
            return "";
        }
    }


    LosLog::LosLog(QObject *parent) : QObject(parent) {}

    LosLog::~LosLog() {}
} /* namespace LosCore */