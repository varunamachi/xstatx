
#include <sys/utsname.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "StatUtils.h"

namespace XStatx {

QHash< QString, QString > StatUtils::readCPUInfo( QFile &cpuInfoFile )
{
    QHash< QString, QString > fields;
    if( cpuInfoFile.exists() && cpuInfoFile.open( QFile::ReadOnly )) {
        QTextStream reader{ &cpuInfoFile };
        auto line = reader.readLine();
        while( ! line.isNull() ) {
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 1 ).trimmed();
                fields.insert( key, value );
            }
            line = reader.readLine();
        }
    } else {
        qWarning() << "Could not open CPUID file at /proc/cpuid";
    }

    return fields;
}

CPUInfo::Architecture StatUtils::getArch()
{
    CPUInfo::Architecture arch = CPUInfo::Architecture::Unknown;
#ifdef Q_OS_LINUX
    utsname vla;
    uname( &vla );
    QString archStr{ vla.machine };
    if( archStr == "x86_64" ) {
        arch = CPUInfo::Architecture::x64;
    }
    else if( archStr == "x86" ) {
        arch = CPUInfo::Architecture::x86;
    }
    else if( archStr == "arm" ) {
        arch = CPUInfo::Architecture::ARM;
    }
    else if( archStr == "arm64" ) {
        arch = CPUInfo::Architecture::ARM64;
    }
#endif
    return arch;
}

bool StatUtils::readFrequencyFromCPUInfo( CPUStatItem< double > &freqs )
{
    QFile cpuIDFile{ "/proc/cpuinfo" };
    double cumulated = 0.0;
    if( cpuIDFile.exists() && cpuIDFile.open( QFile::ReadOnly )) {
        QTextStream reader{ &cpuIDFile };
        auto coreIndex = 0;
        auto line = reader.readLine();
        while( ! line.isNull() ) {
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 1 ).trimmed();
                if( key == "cpu MHz" ) {
                    auto freq = value.toDouble();
                    if( coreIndex == freqs.m_cores.size() ) {
                        freqs.m_cores.push_back( freq );
                    }
                    else {
                        freqs.m_cores[ coreIndex ] = freq;
                    }
                    cumulated += freq;
                    ++ coreIndex;
                }
            }
            line = reader.readLine();
        }
        {
            freqs.m_avg = cumulated / freqs.m_cores.size();
        }
    } else {
        qWarning() << "Could not open CPUID file at /proc/cpuid";
    }
}

bool StatUtils::readCPUUsageInfo( CPUStatItem<CPUUsage> &usage )
{
    return false;
}

bool StatUtils::readCPUTemparature( CPUStatItem<double> &temps )
{
    return false;
}


} // namespace XStatx
