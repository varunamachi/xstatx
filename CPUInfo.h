#pragma once

#include <QObject>

namespace XStatx {

class CPUInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString manufacturer READ manufacturer )
    Q_PROPERTY( Architecture architecture READ architecture )
    Q_PROPERTY( QString modelName READ modelName )
    Q_PROPERTY( int numCores READ numCores )

public:
    enum class Architecture {
        x86,
        x64,
        ARM,
        ARM64,
        Unknown
    };
    Q_ENUM( Architecture )

    CPUInfo( QObject *parent = nullptr )
        : QObject( parent )
        , m_manufacturer( "Unknown" )
        , m_arch( Architecture::Unknown )
        , m_modelName( "N/A" )
        , m_numCores( 0 )
        , m_numThreads( 0 )
    {

    }

    CPUInfo( const QString & manufacturer,
             const Architecture arch,
             const QString &modelName,
             const int numCores,
             const int numThreads,
             QObject *parent = nullptr )
        : QObject( parent )
        , m_manufacturer( manufacturer )
        , m_arch( arch )
        , m_modelName( modelName )
        , m_numCores( numCores )
        , m_numThreads( numThreads )
    {

    }

    const QString & manufacturer() const
    {
        return m_manufacturer;
    }

    Architecture architecture() const
    {
        return m_arch;
    }

    const QString & modelName() const
    {
        return m_modelName;
    }

    int numCores() const
    {
        return m_numCores;
    }

    int numThreads() const
    {
        return m_numThreads;
    }

    static void registerAsQMLType()
    {

    }

private:
    QString m_manufacturer;

    Architecture m_arch;

    QString m_modelName;

    int m_numCores;

    int m_numThreads;

};

}
