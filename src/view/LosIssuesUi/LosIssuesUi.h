#pragma once
#include "common/constants/ConstantsClass.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/style/LosIssuesUi_style.h"
#include <QHeaderView>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

namespace LosView
{

    class LosIssuesUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosIssuesUi(QWidget *parent = nullptr);
        ~LosIssuesUi();

      private slots:
        void onTableDoubleClicked(int row, int colume);
        void onUpdateTable(const QString &file_path, const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags);

      private:
        void initUi();
        void initStyle();
        void initConnect();

      private:
        QTableWidget *L_table = nullptr;
    };

} /* namespace LosView */