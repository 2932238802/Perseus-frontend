#pragma once
#include "common/constants/ConstantsStr/LosStateStr.h"

#include <QHash>
#include <QVariant>
#include <qmutex.h>

namespace LosCore
{

    class LosState
    {
      private:
        LosState()  = default;
        ~LosState() = default;

      public:
        static LosState &instance();
        LosState(const LosState &)            = delete;
        LosState &operator=(const LosState &) = delete;

      public: /* tool */
        template <class T> void set(LosCommon::LosState_Constants::SG_STR key, const T &value)
        {
            QMutexLocker<QMutex> guard(&L_mtx);
            L_data[key] = QVariant::fromValue(value);
        }

        template <class T> T get(LosCommon::LosState_Constants::SG_STR s, const T &default_value = T())
        {
            QMutexLocker<QMutex> guard(&L_mtx);
            if (!L_data.contains(s))
            {
                return default_value;
            }
            return L_data[s].value<T>();
        }

        template <class T> bool contain(LosCommon::LosState_Constants::SG_STR key) const
        {
            QMutexLocker<QMutex> guard(&L_mtx);
            return L_data.contains(key);
        }

        void remove(LosCommon::LosState_Constants::SG_STR key);
        void clear();

      private:
        QHash<LosCommon::LosState_Constants::SG_STR, QVariant> L_data;
        mutable QMutex L_mtx;
    };
} /* namespace LosCore */