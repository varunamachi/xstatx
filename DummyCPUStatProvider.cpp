#include <QString>
#include <QObject>
#include <QtGlobal>
#include <QTime>
#include <QPointer>

#include "CPUInfo.h"
#include "DummyCPUStatProvider.h"

namespace XStatx {

struct DummyCPUStatProvider::Data
{
    Data()
        : m_cpuInfo{ new CPUInfo{ "GinuineIntel",
                                   CPUInfo::Architecture::x64,
                                   "Imaginary x64 Processor",
                                   8,
                                   4 } }
    {

    }

    QPointer< CPUInfo > m_cpuInfo;
};

DummyCPUStatProvider::DummyCPUStatProvider()
    : m_data{ new Data{ }}
{
    qsrand( static_cast< uint >( QTime::currentTime().msec() ));
}

DummyCPUStatProvider::~DummyCPUStatProvider()
{

}

const CPUInfo * DummyCPUStatProvider::getCPUInfo()
{
    return m_data->m_cpuInfo.data();
}

double DummyCPUStatProvider::getCPUUsage() const
{
    return double( qrand() % 100 );
}

double DummyCPUStatProvider::getCPUUsage( std::uint8_t /*coreIndex*/ ) const
{
    return ( double( qrand() % 100 ));
}

double DummyCPUStatProvider::getCPUTemparature() const
{
    return 50 + ( double( qrand() % 10 ));
}

double DummyCPUStatProvider::getCPUTemparature( std::uint8_t /*coreIndex*/ ) const
{
    return 50 + ( double( qrand() % 10 ));
}

double DummyCPUStatProvider::getCPUFrequency() const
{
    return 2000 + ( double( qrand() % 1000 ));
}

double DummyCPUStatProvider::getCPUFrequency(
        std::uint8_t /*coreIndex*/ ) const
{
    return 2000 + ( double( qrand() % 1000 ));
}

void DummyCPUStatProvider::stopCollecting()
{

}

}
