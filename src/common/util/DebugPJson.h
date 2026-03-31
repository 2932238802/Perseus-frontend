#pragma once

#include "core/LosLog/LosLog.h"
#include <QJsonDocument>
#include <qjsonobject.h>

namespace LosCommon
{
inline const void DebugPJson(const QJsonObject &json)
{
    QJsonDocument docP(json);
    QString jsonString = docP.toJson(QJsonDocument::Indented);
    qDebug() << "\n===== JSON START =====";
    qDebug().noquote() << jsonString;
    INF(jsonString, "DebugPJson");
    qDebug() << "===== JSON END =====\n";
}
} // namespace LosCommon