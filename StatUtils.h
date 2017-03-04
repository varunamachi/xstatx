#pragma once

#include <QHash>

#include "CPUInfo.h"

class QFile;

namespace XStatx {

class StatUtils
{
public:
    StatUtils() = delete;
    ~StatUtils() = delete;

    QHash< QString, QString > readCPUInfo( QFile &cpuInfoFile );

    static CPUInfo::Architecture getArch();
};

} // namespace XStatx

