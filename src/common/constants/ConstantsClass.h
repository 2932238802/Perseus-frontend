#pragma once
#include <QString>
#include <string>

namespace LosCommon {

enum DiagnosticSeverity{
    Error = 1,
    Warning,
    Information,
    Hint
};

struct LosDiagnostic {
    int startLine;
    int startChar;
    int endLine;
    int endChar;
    DiagnosticSeverity ds = DiagnosticSeverity::Warning;
    QString message;
};

} // namespace LosCommon