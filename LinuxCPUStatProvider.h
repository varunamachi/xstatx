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

protected:
    CPUInfo * readCPUInfo() override;

    void refreshData( CPUStat &stat ) override;
};

}
