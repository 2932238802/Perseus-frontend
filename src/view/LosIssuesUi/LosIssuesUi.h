// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosLspClass.h"

#include <QTableWidget>
#include <QWidget>
#include <qtmetamacros.h>

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
        void rebuildTable();

      private:
        void initUi();
        void initStyle();
        void initConnect();

      private:
        QTableWidget *L_table = nullptr;
        QMap<QString, QList<LosCommon::LosLsp_Constants::LosDiagnostic>> L_diagMap;
    };

} /* namespace LosView */





