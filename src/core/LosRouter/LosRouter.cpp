
#include "core/LosRouter/LosRouter.h"
#include <qobject.h>


namespace LosCore
{

    LosRouter &LosRouter::instance()
    {
        static LosRouter s;
        return s;
    }

    LosRouter::LosRouter(QObject *parent) : QObject(parent) {}
} /* namespace LosCore */