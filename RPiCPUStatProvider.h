#pragma once

#include "AbstractCPUStatProvider.h"

namespace XStatx {

class RPiCPUStatProvider : public AbstractCPUStatProvider
{
public:
    RPiCPUStatProvider();

protected:
    CPUInfo * readCPUInfo() override;

    void refreshData( CPUStat &stat ) override;
};

}
