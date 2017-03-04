
#include <sys/utsname.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "StatUtils.h"

namespace XStatx {

QHash< QString, QString > StatUtils::readCPUInfo( QFile &cpuInfoFile )
{
    QHash< QString, QString > fields;
    if( cpuInfoFile.exists() && cpuInfoFile.open( QFile::ReadOnly )) {
        QTextStream reader{ &cpuInfoFile };
        auto line = reader.readLine();
        while( ! line.isNull() ) {
            auto comps = line.split( ":" );
            if( comps.size() >= 2 )  {
                auto key = comps.at( 0 ).trimmed();
                auto value = comps.at( 1 ).trimmed();
                fields.insert( key, value );
            }
            line = reader.readLine();
        }
    } else {
        qWarning() << "Could not open CPUID file at /proc/cpuid";
    }

    return fields;
}

CPUInfo::Architecture StatUtils::getArch()
{
    CPUInfo::Architecture arch = CPUInfo::Architecture::Unknown;
#ifdef Q_OS_LINUX
    utsname vla;
    uname( &vla );
    QString archStr{ vla.machine };
    if( archStr == "x86_64" ) {
        arch = CPUInfo::Architecture::x64;
    }
    else if( archStr == "x86" ) {
        arch = CPUInfo::Architecture::x86;
    }
    else if( archStr == "arm" ) {
        arch = CPUInfo::Architecture::ARM;
    }
    else if( archStr == "arm64" ) {
        arch = CPUInfo::Architecture::ARM64;
    }
#endif
    return arch;
}

} // namespace XStatx
