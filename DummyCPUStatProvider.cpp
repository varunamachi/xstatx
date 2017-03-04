#include <QString>
#include <QObject>
#include <QtGlobal>
#include <QTime>
#include <QPointer>

#include "CPUInfo.h"
#include "DummyCPUStatProvider.h"

namespace XStatx {
//struct DummyCPUStatProvider::Data
//{
//    Data()
//        : m_cpuInfo{ new CPUInfo{ "GinuineIntel",
//                                   CPUInfo::Architecture::x64,
//                                   "Imaginary x64 Processor",
//                                   8,
//                                   4 } }
//    {

//    }

//    QPointer< CPUInfo > m_cpuInfo;
//};

CPUInfo * DummyCPUStatProvider::readCPUInfo()
{
    return new CPUInfo { "GinuineIntel",
                CPUInfo::Architecture::x64,
                "Imaginary x64 Processor",
                8,
                4 };
}

void DummyCPUStatProvider::refreshData( CPUStat *stat )
{

}

DummyCPUStatProvider::DummyCPUStatProvider()
{

}

DummyCPUStatProvider::~DummyCPUStatProvider()
{

}

}

