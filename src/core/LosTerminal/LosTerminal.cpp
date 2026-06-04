// Copyright (c) 2026 LosAngelous (shengjie.lin)



#include "LosTerminal.h"


namespace LosCore
{
    /**
     * @brief Construct a new Los Terminal:: Los Terminal object
     * @brief ~LosTerminal
     * 
     * @param parent 
     */
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



    /**
     * @brief onTerminalReady
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



    /**
     * 把 Qt的输出 发射给 term.js
     */
    void LosTerminal::onReadyReadStdOut()
    {
        QByteArray data = L_process->readAllStandardOutput();
        emit _cmd_sendToShell(QString::fromUtf8(data));
    }



    /**
     * @brief 把 Qt的输出 发射给 term.js
     *
     */
    void LosTerminal::onReadyReadStdErr()
    {
        QByteArray data = L_process->readAllStandardError();
        emit _cmd_sendToShell(QString::fromUtf8(data));
    }



    /**
     * @brief onProcessFinished
     * 关闭之后 直接重启
     *
     * @param in a
     * @param in b
     */
    void LosTerminal::onProcessFinished(int a, QProcess::ExitStatus b)
    {
        Q_UNUSED(a);
        Q_UNUSED(b);
        emit _cmd_sendToShell("\r\n[terminal exited, restarting...]\r\n");
        QTimer::singleShot(300, this,
                           [this]()
                           {
                               if (L_process->state() == QProcess::NotRunning)
                               {
                                   onTerminalReady();
                               }
                           });
    }



    /**
     * @brief write
     * 写入
     *
     * @param content
     */
    void LosTerminal::write(const QString &content)
    {
        if (L_process->state() == QProcess::NotRunning)
        {
            onTerminalReady();
            if (!L_process->waitForStarted(1000))
            {
                emit _cmd_sendToShell("\r\n[failed to start terminal]\r\n");
                return;
            }
        }
        if (L_process->state() == QProcess::Running)
        {
            L_process->write(content.toUtf8());
        }
    }



    /**
     * @brief initConnect
     * 初始化 连接
     */
    void LosTerminal::initConnect()
    {
        connect(L_process, &QProcess::readyReadStandardOutput, this, &LosTerminal::onReadyReadStdOut);
        connect(L_process, &QProcess::readyReadStandardError, this, &LosTerminal::onReadyReadStdErr);
        connect(L_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                &LosTerminal::onProcessFinished);
    }
} /* namespace LosCore */