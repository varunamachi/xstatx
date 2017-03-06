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

template< typename ItemType >
struct CPUStatItem
{
    CPUStatItem()
        : m_avg{ ItemType{ }}
    {

    }

    ItemType m_avg;

    QVector< ItemType > m_cores;
};

struct CPUStat
{

    CPUStat()
    {
    }

    CPUStatItem< double > m_temp;

    CPUStatItem< CPUUsage > m_usage;

    CPUStatItem< double > m_frequency;


};

}
