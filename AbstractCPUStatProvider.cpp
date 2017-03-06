
#include <thread>
#include <mutex>

#include "CPUStat.h"
#include "CPUInfo.h"

#include "AbstractCPUStatProvider.h"

namespace XStatx {


struct AbstractCPUStatProvider::Data
{
    Data( AbstractCPUStatProvider *provider )
        : m_provider( provider )
    {
        m_thread = std::thread{
                &Data::refresh,
                this };
    }

    void refresh();

    QPointer< CPUInfo > m_info;

    CPUStat *m_stat;

    CPUStat *m_dupStat;

    bool m_initialized;

    AbstractCPUStatProvider * m_provider;

    std::thread m_thread;

    std::mutex m_mutex;

    bool m_keepRunning;
};
#define LOCK() std::lock_guard< std::mutex > guard( m_data->m_mutex )

AbstractCPUStatProvider::AbstractCPUStatProvider()
{

}

AbstractCPUStatProvider::~AbstractCPUStatProvider()
{
    m_data->m_keepRunning = false;
    m_data->m_thread.join();
}

const CPUInfo * AbstractCPUStatProvider::getCPUInfo()
{
    LOCK();
    return m_data->m_info.data();
}

double AbstractCPUStatProvider::getCPUUsage() const
{
    LOCK();
    return m_data->m_stat->m_avgUsage.m_used;
}

double AbstractCPUStatProvider::getCPUUsage( std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreUsage.size() ) {
        LOCK();
        return m_data->m_stat->m_coreUsage[ coreIndex ].m_used;
    }
    return 0;
}

double AbstractCPUStatProvider::getCPUTemparature() const
{
    LOCK();
    return m_data->m_stat->m_avgTemp;
}

double AbstractCPUStatProvider::getCPUTemparature(
        std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreTemps.size() ) {
        LOCK();
        return m_data->m_stat->m_coreTemps[ coreIndex ];
    }
    return 0;
}

double AbstractCPUStatProvider::getCPUFrequency() const
{
    LOCK();
    return m_data->m_stat->m_avgFrequency;
}

double AbstractCPUStatProvider::getCPUFrequency( std::uint8_t coreIndex ) const
{
    if( coreIndex < m_data->m_stat->m_coreFrequencies.size() ) {
        LOCK();
        return m_data->m_stat->m_coreFrequencies[ coreIndex ];
    }
    return 0;
}

CPUInfo *AbstractCPUStatProvider::readCPUInfo()
{
    return nullptr;
}

void AbstractCPUStatProvider::refreshData( CPUStat &/*stat*/ )
{
    return;
}

void AbstractCPUStatProvider::Data::refresh()
{
    while( m_keepRunning ) {
        if( ! m_initialized ) {
            auto ptr = m_provider->readCPUInfo();
            if( ptr != nullptr ) {
                std::lock_guard< std::mutex > g1{ m_mutex };
                m_info = QPointer< CPUInfo >{ ptr };
            }
        }
        m_provider->refreshData( *m_dupStat );
        std::lock_guard< std::mutex > lock{ m_mutex };
        auto temp = m_dupStat;
        m_dupStat = m_stat;
        m_stat = temp;
    }
}

}
