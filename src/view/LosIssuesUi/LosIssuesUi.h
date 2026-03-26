#pragma once
#include "common/constants/ConstantsClass.h"
#include "view/style/LosIssuesUi_style.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QHeaderView>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

namespace LosView {

class LosIssuesUi : public QWidget {
  Q_OBJECT

public:
  explicit LosIssuesUi(QWidget *parent = nullptr);
  ~LosIssuesUi();

  void updateTable(const QString &file_path,
                   const QList<LosCommon::LosDiagnostic> &diags);

private slots:
  void onTableDoubleClicked(int row, int colume);

private:
  void initUi();
  void initStyle();
  void initConnect();

private:
  QTableWidget *L_table = nullptr;
};

} // namespace LosView