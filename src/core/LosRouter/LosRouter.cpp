// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "core/LosRouter/LosRouter.h"


namespace LosCore
{

    LosRouter &LosRouter::instance()
    {
        static LosRouter s;
        return s;
    }

    LosRouter::LosRouter(QObject *parent) : QObject(parent) {}
} /* namespace LosCore */