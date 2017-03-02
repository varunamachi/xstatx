#pragma once

#include <memory>

#include <QString>

#include "ICPUStatProvider.h"

namespace XStatx {

class CPUInfo;

class CPUStatProvider : public ICPUStatProvider
{
public:
    const CPUInfo * getCPUInfo() override;

    double getCPUUsage() const override;

    double getCPUUsage( std::uint8_t coreIndex ) const override;

    double getCPUTemparature() const override;

    double getCPUTemparature( std::uint8_t coreIndex ) const override;

    double getCPUFrequency() const override;

    double getCPUFrequency( std::uint8_t coreIndex ) const override;

    CPUStatProvider();

    ~CPUStatProvider();

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};

}
