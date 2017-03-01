#include <mutex>

#include <sys/utsname.h>

#include <QThread>
#include <QVector>
#include <QReadWriteLock>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPointer>

#include "CPUStatProvider.h"
#include "CPUInfo.h"

#define LOCK() std::lock_guard< std::mutex > guard( m_data->m_lock )

namespace XStatx {

struct CPUUsage
{
    CPUUsage()
        : m_used{ 0 }
        , m_idle{ 0 }
        , m_total{ 0 }
    {
    }

    double m_used;

    double m_idle;

    double m_total;
};

struct CPUStatProvider::Data
{
    QPointer< CPUInfo > m_cpuInfo;

    double m_avgTemp;

    CPUUsage m_avgUsage;

    double m_avgFrequency;

    QVector< double > m_coreTemps;

    QVector< CPUUsage > m_coreUsage;

    QVector< double > m_coreFrequencies;

    std::mutex m_lock;

    void readCPUID();

    void updateFrequency();

    void updateTemparature();

    void updateCPUUsage();

    void refresh();

    Data()
    {
        readCPUID();
        refresh();
    }
};

static CPUInfo::Architecture getArch()
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


const CPUInfo * CPUStatProvider::getCPUInfo()
{
    LOCK();
    return m_data->m_cpuInfo.data();
}

double CPUStatProvider::getCPUUsage() const
{
    return m_data->m_avgUsage.m_used;
}

double CPUStatProvider::getCPUUsage( std::uint8_t coreIndex ) const
{
    LOCK();
    if( coreIndex < m_data->m_coreUsage.size() ) {
        return m_data->m_coreUsage[ coreIndex ].m_used;
    }
    return 0;
}

double CPUStatProvider::getCPUTemparature() const
{
    LOCK();
    return m_data->m_avgTemp;
}

double CPUStatProvider::getCPUTemparature( std::uint8_t coreIndex ) const
{
    LOCK();
    if( coreIndex < m_data->m_coreTemps.size() ) {
        return m_data->m_coreTemps[ coreIndex ];
    }
    return 0;
}

void CPUStatProvider::Data::readCPUID()
{
    QString vendorId{ };
    QString modelName{ };
    int numCores{ 1 };
    int numThreads{ 1 };

    QFile cpuIDFile{ "/proc/cpuid" };
    if( cpuIDFile.exists() && cpuIDFile.open( QFile::ReadOnly )) {
        QTextStream reader{ &cpuIDFile };
        bool oneCoreDone = false;
        while( ! reader.atEnd() ) {
            auto line = reader.readLine();
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 0 ).trimmed();
                if( key == "processor" ) {
                    if( oneCoreDone ) {
                        break;
                    }
                    oneCoreDone = true;
                }
                if( key == "vendor_id" ) {
                    vendorId = value;
                }
                else if( key == "model name" ) {
                    modelName = value;
                }
                else if( key == "cpu cores" ) {
                    numCores = value.toInt();
                }
                else if( key == "siblings" ) {
                    numThreads = value.toInt();
                }
            }
        }
        auto arch = getArch();
        {
            std::lock_guard< std::mutex > guard( m_lock );
            m_cpuInfo = new CPUInfo{ vendorId,
                    arch,
                    modelName,
                    numCores,
                    numThreads };
        }
    } else {
        qWarning() << "Could not open CPUID file at /proc/cpuid";
    }
}

void CPUStatProvider::Data::updateFrequency()
{
    QFile cpuIDFile{ "/proc/cpuid" };
    double cumulated = 0.0;
    if( cpuIDFile.exists() && cpuIDFile.open( QFile::ReadOnly )) {
        QTextStream reader{ &cpuIDFile };
        auto coreIndex = 0;
        while( ! reader.atEnd() ) {
            auto line = reader.readLine();
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 0 ).trimmed();
                if( key == "cpu MHz" ) {
                    std::lock_guard< std::mutex > guard( m_lock );
                    auto freq = value.toDouble();
                    if( coreIndex < m_coreFrequencies.size() ) {
                        m_coreFrequencies[ coreIndex ] = freq;
                    }
                    else {
                        m_coreFrequencies.push_back( freq );
                    }
                    cumulated += freq;
                }
            }
        }
        {
            std::lock_guard< std::mutex > guard( m_lock );
            m_avgFrequency = cumulated / m_coreFrequencies.size();
        }
    } else {
        qWarning() << "Could not open CPUID file at /proc/cpuid";
    }
}

void CPUStatProvider::Data::updateTemparature()
{
    std::lock_guard< std::mutex > guard( m_lock );
}

void CPUStatProvider::Data::updateCPUUsage()
{
//    from: http://stackoverflow.com/a/28063368
//    user nice system idle iowait irq softirq  steal  guest  guest_nice
//    Idle = idle + iowait
//    NonIdle=user+nice+system+irq+softirq+steal
//    Total=Idle+NonIdle # first line of file for all cpus
//    CPU_Percentage=((Total-PrevTotal)-(Idle-PrevIdle))/(Total-PrevTotal)

    auto calc = [ = ]( int coreIndex, QString &line ) {
//        auto comps = line.split( "")
        QTextStream stream{ &line };
        double user = 0.0;
        double nice = 0.0;
        double system = 0.0;
        double idle = 0.0;
        double iowait = 0.0;
        double irq = 0.0;
        double softirq = 0.0;
        double steal = 0.0;
        stream  >> user
                >> nice
                >> system
                >> idle
                >> iowait
                >> irq
                >> softirq
                >> steal;
        auto nonIdle = user + nice + system + irq + softirq + steal;
        auto totalIdle = idle + iowait;
        auto total = idle + nonIdle;
        auto &cur= m_avgUsage;
        if( coreIndex != -1 && coreIndex < m_coreUsage.size() ) {
            if( coreIndex == m_coreUsage.size() ) {
                m_coreUsage.push_back( CPUUsage{ });
            }
            cur = m_coreUsage[ coreIndex ];
        }
        auto pc = (( total - cur.m_total ) - ( totalIdle - cur.m_idle ))
                / ( total - cur.m_total );
        cur.m_used = pc;
        cur.m_total = total;
        cur.m_idle = totalIdle;
    };
    QFile statFile{ "/proc/stat" };
    auto isFirst = true;
    auto coreIndex = 0;
    if( statFile.exists() ) {
        QTextStream stream{ &statFile };
        while( ! stream.atEnd() ) {
            auto line = stream.readLine();
            if( isFirst && line.startsWith( "cpu" )) {
                calc( -1, line );
                isFirst = false;
            }
            else if( line.startsWith( "cpu" )) {
                calc( coreIndex, line );
                ++ coreIndex;
            }
            else {
                break;
            }
        }
    }
}

void CPUStatProvider::Data::refresh()
{
    updateFrequency();
    updateTemparature();
}

}
