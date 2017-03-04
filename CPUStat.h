#pragma once

#include <QPointer>
#include <QVector>

#include "CPUInfo.h"


namespace XStatx {

struct CPUUsage
{
    CPUUsage()
        : m_used{ 0 }
        , m_idle{ 0 }
        , m_total{ 0 }
    {
    }

    double m_used;

    double m_idle;

    double m_total;
};

struct CPUStat
{

    CPUStat()
        : m_avgTemp{ 0 }
        , m_avgUsage{ }
        , m_avgFrequency{ 0 }
        , m_keepRunning{ true }
    {
    }

    double m_avgTemp;

    CPUUsage m_avgUsage;

    double m_avgFrequency;

    bool m_keepRunning;

    QVector< double > m_coreTemps;

    QVector< CPUUsage > m_coreUsage;

    QVector< double > m_coreFrequencies;
};

}
