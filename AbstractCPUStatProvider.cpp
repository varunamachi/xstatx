
#include "CPUStat.h"
#include "CPUInfo.h"

#include "AbstractCPUStatProvider.h"

namespace XStatx {

struct AbstractCPUStatProvider::Data
{
    QPointer< CPUInfo > m_info;

    std::unique_ptr< CPUStat > m_stat;
};

const CPUInfo * AbstractCPUStatProvider::getCPUInfo()
{
    return m_data->m_info.data();
}

double AbstractCPUStatProvider::getCPUUsage() const
{
    return m_data->m_stat->m_avgUsage.m_used;
}

double AbstractCPUStatProvider::getCPUUsage( std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreUsage.size() ) {
        return m_data->m_stat->m_coreUsage[ coreIndex ].m_used;
    }
    return 0;
}

double AbstractCPUStatProvider::getCPUTemparature() const
{
    return m_data->m_stat->m_avgTemp;
}

double AbstractCPUStatProvider::getCPUTemparature(
        std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreTemps.size() ) {
        return m_data->m_stat->m_coreTemps[ coreIndex ];
    }
    return 0;
}

double AbstractCPUStatProvider::getCPUFrequency() const
{
    return m_data->m_stat->m_avgFrequency;
}

double AbstractCPUStatProvider::getCPUFrequency( std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreFrequencies.size() ) {
        return m_data->m_stat->m_coreFrequencies[ coreIndex ];
    }
    return 0;
}

}
