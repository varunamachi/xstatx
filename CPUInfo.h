
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
    enum Architecture {
        Arch_x86,
        Arch_x64,
        Arch_ARM,
        Arch_ARM64,
        Arch_Unknown
    };

    CPUInfo( QObject *parent = nullptr )
        : QObject( parent )
        , m_manufacturer( "Unknown" )
        , m_arch( Arch_Unknown )
        , m_modelName( "N/A" )
        , m_numCores( 0 )
    {

    }

    CPUInfo( const QString & manufacturer,
             const Architecture arch,
             const QString &modelName,
             const int numCores,
             QObject *parent = nullptr )
        : QObject( parent )
        , m_manufacturer( manufacturer )
        , m_arch( arch )
        , m_modelName( modelName )
        , m_numCores( numCores )
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

    static void registerAsQMLType()
    {

    }

private:
    QString m_manufacturer;

    Architecture m_arch;

    QString m_modelName;

    int m_numCores;

};

}
