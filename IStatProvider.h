#pragma once

namespace XStat {

struct IStatProvider {
    virtual int numCPUCOres() const = 0;

    virtual double cpuCoreTemparature( int coreID ) = 0;

    virtual double cpuUsage() = 0;

    virtual double cpuCoreUsage( int coreID ) = 0;



};

}
