#pragma once

#include <memory>

#include "ICPUStatProvider.h"

namespace XStatx {

class DummyCPUStatProvider : public ICPUStatProvider
{
public:
    DummyCPUStatProvider();

    ~DummyCPUStatProvider();

    const CPUInfo *getCPUInfo() override;

    double getCPUUsage() const override;

    double getCPUUsage( std::uint8_t coreIndex ) const override;

    double getCPUTemparature() const override;

    double getCPUTemparature( std::uint8_t coreIndex ) const override;

    double getCPUFrequency() const override;

    double getCPUFrequency( std::uint8_t coreIndex ) const override;

    void stopCollecting() override;

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};

}
