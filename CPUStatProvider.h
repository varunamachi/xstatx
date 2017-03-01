#pragma once

#include <memory>

#include <QString>

namespace XStatx {

class CPUInfo;

class CPUStatProvider
{
public:
    const CPUInfo * getCPUInfo();

    double getCPUUsage() const;

    double getCPUUsage( std::uint8_t coreIndex ) const;

    double getCPUTemparature() const;

    double getCPUTemparature( std::uint8_t coreIndex ) const;

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};

}
