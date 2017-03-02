#include <QFile>
#include <QDebug>
#include <QDateTime>

#include "StatProvider.h"
#include "ICPUStatProvider.h"

namespace XStatx {

struct StatProvider::Data
{
    Data( std::unique_ptr< ICPUStatProvider > &&cpuStatProvider )
        : m_cpuStatProvider{ std::move( cpuStatProvider )}
    {

    }

    std::unique_ptr< ICPUStatProvider > m_cpuStatProvider;
};

StatProvider::StatProvider(
        std::unique_ptr< ICPUStatProvider > &&cpuStatProvider,
        QObject *parent )
    : QObject(parent)
    , m_data{ new Data{ std::move( cpuStatProvider )}}
{
//    qsrand( static_cast< uint >( QTime::currentTime().msec() ));
}

StatProvider::~StatProvider()
{

}

const CPUInfo *StatProvider::getCPUInfo()
{
    return m_data->m_cpuStatProvider->getCPUInfo();
}

double StatProvider::getCPUUsage() const
{
    return m_data->m_cpuStatProvider->getCPUUsage();
}

double StatProvider::getCoreUsage( std::uint8_t coreIndex ) const
{
    return m_data->m_cpuStatProvider->getCPUUsage( coreIndex );
}

double StatProvider::getCPUTemparature() const
{
    return m_data->m_cpuStatProvider->getCPUFrequency();
}

double StatProvider::getCoreTemparature(uint8_t coreIndex) const
{
    return m_data->m_cpuStatProvider->getCPUFrequency( coreIndex );
}

double StatProvider::getCPUFrequency() const
{
    return m_data->m_cpuStatProvider->getCPUFrequency();
}

double StatProvider::getCoreFrequency( std::uint8_t coreIndex ) const
{
    return m_data->m_cpuStatProvider->getCPUFrequency( coreIndex );
}



}
