#pragma once

#include <memory>

#include <QString>

#include "ICPUStatProvider.h"

namespace XStatx {

class CPUInfo;

class LinuxCPUStatProvider : public ICPUStatProvider
{
public:
    const CPUInfo * getCPUInfo() override;

    double getCPUUsage() const override;

    double getCPUUsage( std::uint8_t coreIndex ) const override;

    double getCPUTemparature() const override;

    double getCPUTemparature( std::uint8_t coreIndex ) const override;

    double getCPUFrequency() const override;

    double getCPUFrequency( std::uint8_t coreIndex ) const override;

    void stopCollecting() override;

    LinuxCPUStatProvider();

    ~LinuxCPUStatProvider();

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};

}
