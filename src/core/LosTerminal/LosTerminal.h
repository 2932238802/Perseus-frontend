

#pragma once
#include "common/constants/ConstantsStr/LosTerminalStr.h"
#include "core/LosRouter/LosRouter.h"
#include <QTimer>
#include <QToolButton>
#include <qglobal.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>
#include <qtoolbutton.h>
#include <qwidget.h>

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
        void onProcessFinished(int, QProcess::ExitStatus);

      private: /* init */
        void initConnect();

      private:
        QProcess *L_process;

      signals:
        void _cmd_sendToShell(const QString &);
    };

} /* namespace LosCore */
