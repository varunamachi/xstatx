
#include "CPUInfo.h"
#include "RPiCPUStatProvider.h"

namespace XStatx {

RPiCPUStatProvider::RPiCPUStatProvider()
{

}

CPUInfo * RPiCPUStatProvider::readCPUInfo()
{
    return new CPUInfo { "GinuineIntel",
                CPUInfo::Architecture::x64,
                "Imaginary x64 Processor",
                8,
                4 };
}

void RPiCPUStatProvider::refreshData( CPUStat &stat )
{

}

}
