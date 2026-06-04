// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosState.h"
namespace LosCore
{
    /**
     * @brief 单例模式
     * 
     * @return LosState& 
     */
    LosState &LosState::instance()
    {
        static LosState s;
        return s;
    }



    /**
     * @brief 移除全局状态信息
     * 
     * @param key 
     */
    void LosState::remove(LosCommon::LosState_Constants::SG_STR key)
    {
        QMutexLocker locker(&L_mtx);
        L_data.remove(key);
    }



    /**
     * @brief 清理 全局状态信息
     */
    void LosState::clear()
    {
        QMutexLocker locker(&L_mtx);
        L_data.clear();
    }
} /* namespace LosCore */
