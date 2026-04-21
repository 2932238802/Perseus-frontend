#pragma once
#include "common/constants/ConstantsNum/LosCompleterUiNum.h"
#include "view/style/LosCompleterUi_style.h"
#include <QAbstractItemView>
#include <QCompleter>
#include <QStringListModel>
#include <qobject.h>
#include <qtmetamacros.h>

namespace LosView
{
    class LosCompleterUi : public QCompleter
    {
        Q_OBJECT
      public:
        explicit LosCompleterUi(QObject *parent = nullptr);
        ~LosCompleterUi();

      public: /* set */
        void updateCompletionList(const QStringList &);

      private: /* init */
        void initStyle();

      private:
        QStringListModel *L_model = nullptr;
    };
} /* namespace LosView */