#pragma once

#include <QHash>

#include "CPUInfo.h"
#include "CPUStat.h"

class QFile;

namespace XStatx {

class StatUtils
{
public:
    StatUtils() = delete;
    ~StatUtils() = delete;

    static QHash< QString, QString > readCPUInfo( QFile &cpuInfoFile );

    static CPUInfo::Architecture getArch();

    static bool readFrequencyFromCPUInfo( CPUStatItem< double > &freqs );

    static bool readCPUUsageInfo( CPUStatItem< CPUUsage > &usage );

    static bool readCPUTemparature( CPUStatItem< double > &temps );
};

} // namespace XStatx

