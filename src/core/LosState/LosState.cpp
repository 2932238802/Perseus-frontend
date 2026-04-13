#include "LosState.h"

namespace LosCore
{
    LosState &LosState::instance()
    {
        static LosState s;
        return s;
    }



    void LosState::remove(LosCommon::LosState_Constants::SG_STR key)
    {
        QMutexLocker locker(&L_mtx);
        L_data.remove(key);
    }



    void LosState::clear()
    {
        QMutexLocker locker(&L_mtx);
        L_data.clear();
    }
} /* namespace LosCore */
