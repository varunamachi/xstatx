#include <mutex>

#include <QThread>
#include <QVector>
#include <QReadWriteLock>

#include "CPUStatProvider.h"
#include "CPUInfo.h"

#define LOCK() std::lock_guard< std::mutex > guard( m_data->m_lock )

namespace XStatx {

struct CPUStatProvider::Data
{
    CPUInfo m_cpuInfo;

    double m_avgTemp;

    double m_avgCPUUsage;

    QVector< double > m_coreTemps;

    QVector< double > m_coreUsage;

    std::mutex m_lock;

    void updateCPUID();

    void updateTemparature();

    void refresh();
};

const CPUInfo &CPUStatProvider::getCPUInfo()
{
    LOCK();
    return m_data->m_cpuInfo;
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

void CPUStatProvider::Data::updateCPUID()
{
    std::lock_guard< std::mutex > guard( m_lock );

}

void CPUStatProvider::Data::updateTemparature()
{
    std::lock_guard< std::mutex > guard( m_lock );
}

void CPUStatProvider::Data::refresh()
{
    updateCPUID();
    updateTemparature();
}

}
