#include <mutex>

#if defined( Q_OS_LINUX )
    #include <sys/utsname.h>
#elif defined (Q_OS_WINDOWS)
#endif

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

struct CPUStatProvider::Data
{
    QPointer< CPUInfo > m_cpuInfo;

    double m_avgTemp;

    double m_avgCPUUsage;

    double m_avgFrequency;

    QVector< double > m_coreTemps;

    QVector< double > m_coreUsage;

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
    QString archStr{ vla };
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
    return m_data->m_avgCPUUsage;
}

double CPUStatProvider::getCPUUsage( std::uint8_t coreIndex ) const
{
    LOCK();
    if( coreIndex < m_data->m_coreUsage.size() ) {
        return m_data->m_coreUsage[ coreIndex ];
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
        while( ! reader.atEnd() ) {
            auto line = reader.readLine();
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 0 ).trimmed();
                if( key == "cpu MHz" ) {
                    std::lock_guard< std::mutex > guard( m_lock );
                    auto freq = value.toDouble();
                    m_coreFrequencies.push_back( freq );
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

void CPUStatProvider::Data::refresh()
{
    updateFrequency();
    updateTemparature();
}

}
