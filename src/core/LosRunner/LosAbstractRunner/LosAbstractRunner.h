
#pragma once

#include <QObject>
#include <qtmetamacros.h>

namespace LosCore
{
    class LosAbstractRunner : public QObject
    {
        Q_OBJECT
      public:
        explicit LosAbstractRunner(QObject *parent = nullptr);
        virtual ~LosAbstractRunner()                           = default;
        virtual void start(const QString &main_file_path = "") = 0;
        virtual void stop()                                    = 0;
      private:
    };
} /* namespace LosCore */