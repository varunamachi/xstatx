#pragma once

#include <memory>

#include <QString>

#include "AbstractCPUStatProvider.h"

namespace XStatx {

class CPUInfo;

class LinuxCPUStatProvider : public AbstractCPUStatProvider
{
public:

    LinuxCPUStatProvider();

    ~LinuxCPUStatProvider();

//    void readCPUInfo();

//    void updateFrequency();

//    void updateTemparature();

//    void updateCPUUsage();

//    void refresh();

protected:
    CPUInfo * readCPUInfo() override;

    void refreshData( CPUStat *stat ) override;
};

}
