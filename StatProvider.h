#pragma once

#include <memory>

#include <QObject>

class QQmlEngine;
class QJSEngine;

namespace XStatx {

class CPUInfo;
class ICPUStatProvider;

class StatProvider final : public QObject
{
    Q_OBJECT
public:
    explicit StatProvider(
            std::unique_ptr< ICPUStatProvider > &&cpuStatProvider,
            QObject *parent = 0 );

    ~StatProvider();

    Q_INVOKABLE const CPUInfo * getCPUInfo();

    Q_INVOKABLE double getCPUUsage() const;

    Q_INVOKABLE double getCoreUsage( std::uint8_t coreIndex ) const;

    Q_INVOKABLE double getCPUTemparature() const;

    Q_INVOKABLE double getCoreTemparature( std::uint8_t coreIndex ) const;

    Q_INVOKABLE double getCPUFrequency() const;

    Q_INVOKABLE double getCoreFrequency( std::uint8_t coreIndex ) const;

    Q_INVOKABLE void stopCollecting();

private:
    struct Data;
    std::unique_ptr< Data > m_data;
};


}
