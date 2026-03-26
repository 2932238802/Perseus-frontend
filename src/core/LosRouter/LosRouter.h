

#pragma once

#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>

#include "common/constants/ConstantsClass.h"

namespace LosCore
{
class LosRouter : public QObject
{
    Q_OBJECT
  public:
    static LosRouter &instance();

  private:
    explicit LosRouter(QObject *parent = nullptr);
    ~LosRouter() = default;


  signals:
    void _cmd_whereDefine(int line, int col, const QString &file_path);

    void _cmd_lsp_request_openFile(const QString &file_path, const QString &file_content);
    void _cmd_lsp_request_textChanged(const QString &file_path, const QString &file_content);
    void _cmd_lsp_request_completeion(const QString &file_path, int line, int col);
    void _cmd_lsp_msg_didChangeWatchedFiles(const QString &,
                                            LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type);

    void _cmd_lsp_result_completion(const QStringList &words);
    void _cmd_lsp_result_diagnostics(const QString &file_path,
                                     const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &diags);
    void _cmd_lsp_result_definition(const QString &file_name, int line);

    void _cmd_gotoFile(const QString &file_path, int line);
    void _cmd_codeFormat();
    void _cmd_fileDirty(bool dirty);

    void _cmd_fileSystemChanged();
};
} // namespace LosCore