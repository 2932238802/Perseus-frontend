
#include "LosFormatBase.h"
#include <qprocess.h>
namespace LosCore
{
LosFormatBase::LosFormatBase(QObject *parent) : QObject{parent}
{
    L_process = new QProcess(this);
}

} // namespace LosCore