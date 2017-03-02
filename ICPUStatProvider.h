#pragma once

#include <cstdint>

namespace XStatx {

class CPUInfo;

struct ICPUStatProvider {

    virtual const CPUInfo * getCPUInfo() = 0;

    virtual double getCPUUsage() const = 0;

    virtual double getCPUUsage( std::uint8_t coreIndex ) const = 0;

    virtual double getCPUTemparature() const = 0;

    virtual double getCPUTemparature( std::uint8_t coreIndex ) const = 0;

    virtual double getCPUFrequency() const = 0;

    virtual double getCPUFrequency( std::uint8_t coreIndex ) const = 0;

    virtual ~ICPUStatProvider() {}
};

}
