#pragma once

#include <memory>

#include "AbstractCPUStatProvider.h"

namespace XStatx {

class DummyCPUStatProvider : public AbstractCPUStatProvider
{
public:
    DummyCPUStatProvider();

    ~DummyCPUStatProvider();

protected:
    CPUInfo * readCPUInfo() override;

    void refreshData( CPUStat &stat ) override;
};

}
