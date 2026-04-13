

#pragma once
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore
{
    class LosTerminal : public QObject
    {
        Q_OBJECT
      public:
        explicit LosTerminal(QObject *parent = nullptr);
        ~LosTerminal();

      public slots:
        void write(const QString &);
        void onTerminalReady();
        void onReadyReadStdOut();
        void onReadyReadStdErr();

      private: /* init */
        void initConnect();

      private:
        QProcess *L_process;

      signals:
        void _cmd_sendToShell(const QString &);
    };

} /* namespace LosCore */
