

#include "LosTerminal.h"
#include "core/LosRouter/LosRouter.h"
#include <qglobal.h>
#include <qobject.h>
#include <qprocess.h>

namespace LosCore
{
    LosTerminal::LosTerminal(QObject *parent) : QObject(parent)
    {
        L_process = new QProcess(this);
        initConnect();
    };


    LosTerminal::~LosTerminal()
    {
        if (L_process->state() == QProcess::Running)
        {
            L_process->kill();
            L_process->waitForFinished(1000);
        }
    }



    /*
     * - 终端准备就绪
     */
    void LosTerminal::onTerminalReady()
    {
        if (L_process->state() != QProcess::NotRunning)
        {
            return;
        }
#ifdef Q_OS_WIN
        L_process->start("powershell.exe");
#else
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("TERM", "xterm-256color");
        L_process->setProcessEnvironment(env);
        L_process->start("script", QStringList() << "-q" << "/dev/null");
#endif
    }


    /*
     * - 把 Qt的输出 发射给 term.js
     */
    void LosTerminal::onReadyReadStdOut()
    {
        QByteArray data = L_process->readAllStandardOutput();
        emit _cmd_sendToShell(QString::fromUtf8(data));
    }



    /*
     * - 把 Qt的输出 发射给 term.js
     */
    void LosTerminal::onReadyReadStdErr()
    {
        QByteArray data = L_process->readAllStandardError();
        emit _cmd_sendToShell(QString::fromUtf8(data));
    }



    /*
     * - write QByteArray
     * -
     */
    void LosTerminal::write(const QString &content)
    {
        if (L_process->state() == QProcess::Running)
        {
            L_process->write(content.toUtf8());
        }
    }



    /*
     * - 初始化链接
     */
    void LosTerminal::initConnect()
    {
        connect(L_process, &QProcess::readyReadStandardOutput, this, &LosTerminal::onReadyReadStdOut);
        connect(L_process, &QProcess::readyReadStandardError, this, &LosTerminal::onReadyReadStdErr);
    }

} /* namespace LosCore */