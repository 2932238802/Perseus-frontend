

#include "view/LosIssuesUi/LosIssuesUi.h"



namespace LosView
{
    /*
     * construct
     */
    LosIssuesUi::LosIssuesUi(QWidget *parent) : QWidget(parent)
    {
        initUi();
        initStyle();
        initConnect();
    }
    LosIssuesUi::~LosIssuesUi() {}



    /*
     * - 更新下册的
     */
    void LosIssuesUi::onUpdateTable(const QString &file_path,
                                    const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags)
    {
        if (!L_table)
            return;

        L_table->setRowCount(diags.size());
        for (int i = 0; i < diags.size(); i++)
        {
            const auto &diag = diags[i];
            L_table->insertRow(i);
            bool isErr               = diag.ds == LosCommon::LosLsp_Constants::DiagnosticSeverity::Error;
            QString level            = isErr ? "Err" : "War";
            QTableWidgetItem *item_1 = new QTableWidgetItem(level);
            item_1->setForeground(isErr ? Qt::red : QColor(255, 165, 0));
            QTableWidgetItem *item_2 = new QTableWidgetItem(diag.message);
            QTableWidgetItem *item_3 = new QTableWidgetItem(QString::number(diag.startLine + 1));
            item_3->setData(Qt::UserRole, diag.startLine);
            QTableWidgetItem *item_4 = new QTableWidgetItem(LosModel::LosFilePath(file_path).getFileName());
            item_4->setData(Qt::UserRole, file_path);

            L_table->setItem(i, 0, item_1);
            L_table->setItem(i, 1, item_2);
            L_table->setItem(i, 2, item_3);
            L_table->setItem(i, 3, item_4);
        }
    }



    /*
     * -
     */
    void LosIssuesUi::onTableDoubleClicked(int row, int colume)
    {
        int targetLine     = L_table->item(row, 2)->data(Qt::UserRole).toInt();
        QString targetFile = L_table->item(row, 3)->data(Qt::UserRole).toString();
        emit LosCore::LosRouter::instance()._cmd_gotoFile(targetFile, targetLine);
    }



    /*
     * - 初始化
     */
    void LosIssuesUi::initUi()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        L_table = new QTableWidget(this);
        L_table->setColumnCount(4);
        L_table->setHorizontalHeaderLabels({"Level", "Message", "Line", "File"});
        layout->addWidget(L_table);
        L_table->setSelectionBehavior(QAbstractItemView::SelectRows);
        L_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        L_table->setShowGrid(false);
        L_table->horizontalHeader()->setStretchLastSection(true);
        L_table->verticalHeader()->setVisible(false);
        L_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        L_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    }



    /*
     * - 样式
     */
    void LosIssuesUi::initStyle()
    {
        this->L_table->setStyleSheet(LosStyle::LosIssuesUi_getStyle());
    }



    /*
     * - 连接
     */
    void LosIssuesUi::initConnect()
    {
        if (L_table)
            connect(L_table, &QTableWidget::cellDoubleClicked, this, &LosIssuesUi::onTableDoubleClicked);
        connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_lsp_result_diagnostics, this,
                &LosIssuesUi::onUpdateTable);
    }

} /* namespace LosView */