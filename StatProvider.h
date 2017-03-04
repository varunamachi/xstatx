#pragma once

#include <memory>

#include <QObject>

class QQmlEngine;
class QJSEngine;

namespace XStatx {

class CPUInfo;
class AbstractCPUStatProvider;

class StatProvider final : public QObject
{
    Q_OBJECT
public:
    explicit StatProvider(
            std::unique_ptr< AbstractCPUStatProvider > &&cpuStatProvider,
            QObject *parent = 0 );

    ~StatProvider();

    Q_INVOKABLE const CPUInfo * getCPUInfo();

    Q_INVOKABLE double getCPUUsage() const;

    Q_INVOKABLE double getCoreUsage( quint8 coreIndex ) const;

    Q_INVOKABLE double getCPUTemparature() const;

    Q_INVOKABLE double getCoreTemparature( quint8 coreIndex ) const;

    Q_INVOKABLE double getCPUFrequency() const;

    Q_INVOKABLE double getCoreFrequency( quint8 coreIndex ) const;

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};


}
