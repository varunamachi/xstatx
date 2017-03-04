#pragma once

#include <QPointer>

#include <cstdint>
#include <memory>

namespace XStatx {

class CPUInfo;
struct CPUStat;

class AbstractCPUStatProvider {

public:
    const CPUInfo * getCPUInfo();

    double getCPUUsage() const;

    double getCPUUsage( std::uint8_t coreIndex ) const;

    double getCPUTemparature() const;

    double getCPUTemparature( std::uint8_t coreIndex ) const;

    double getCPUFrequency() const;

    double getCPUFrequency( std::uint8_t coreIndex ) const;

    virtual ~AbstractCPUStatProvider();

protected:
    struct Data;
    std::unique_ptr< Data > m_data;

    virtual CPUInfo * readCPUInfo() = 0;

    virtual void refreshData( CPUStat *stat ) = 0;

private:

};

}
