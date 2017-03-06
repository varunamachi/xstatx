#pragma once

#include <QPointer>

#include <cstdint>
#include <memory>

namespace XStatx {

class CPUInfo;
struct CPUStat;

class AbstractCPUStatProvider {

public:
    AbstractCPUStatProvider();

    virtual ~AbstractCPUStatProvider() = 0;

    const CPUInfo * getCPUInfo();

    double getCPUUsage() const;

    double getCPUUsage( std::uint8_t coreIndex ) const;

    double getCPUTemparature() const;

    double getCPUTemparature( std::uint8_t coreIndex ) const;

    double getCPUFrequency() const;

    double getCPUFrequency( std::uint8_t coreIndex ) const;


protected:
    struct Data;
    std::unique_ptr< Data > m_data;

    virtual CPUInfo * readCPUInfo();

    virtual void refreshData( CPUStat &stat );

private:

};

}
