#include <mutex>
#include <thread>

#include <QVector>
#include <QReadWriteLock>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPointer>

#include "LinuxCPUStatProvider.h"
#include "CPUInfo.h"


namespace XStatx {

LinuxCPUStatProvider::LinuxCPUStatProvider()
    : AbstractCPUStatProvider{}
{

}

LinuxCPUStatProvider::~LinuxCPUStatProvider()
{

}

CPUInfo * LinuxCPUStatProvider::readCPUInfo()
{
    return new CPUInfo { "GinuineIntel",
                CPUInfo::Architecture::x64,
                "Imaginary x64 Processor",
                8,
                4 };
}

void LinuxCPUStatProvider::refreshData( CPUStat &stat )
{

}

//void LinuxCPUStatProvider::readCPUInfo()
//{
//    QString vendorId{ };
//    QString modelName{ };
//    int numCores{ 1 };
//    int numThreads{ 1 };

//    QFile cpuIDFile{ "/proc/cpuinfo" };
//    if( cpuIDFile.exists() && cpuIDFile.open( QFile::ReadOnly )) {
//        QTextStream reader{ &cpuIDFile };
//        bool oneCoreDone = false;
//        auto line = reader.readLine();
//        while( ! line.isNull() ) {
//            auto comps = line.split( ":" );
//            if( comps.size() >= 2 )  {
//                auto key = comps.at( 0 ).trimmed();
//                auto value = comps.at( 1 ).trimmed();
//                if( key == "processor" ) {
//                    if( oneCoreDone ) {
//                        break;
//                    }
//                    oneCoreDone = true;
//                }
//                if( key == "vendor_id" ) {
//                    vendorId = value;
//                }
//                else if( key == "model name" ) {
//                    modelName = value;
//                }
//                else if( key == "cpu cores" ) {
//                    numCores = value.toInt();
//                }
//                else if( key == "siblings" ) {
//                    numThreads = value.toInt();
//                }
//            }
//            line = reader.readLine();
//        }
//        auto arch = getArch();
//        {
//            std::lock_guard< std::mutex > guard( m_lock );
//            m_cpuInfo = new CPUInfo{ vendorId,
//                    arch,
//                    modelName,
//                    numCores,
//                    numThreads };
//        }
//    } else {
//        qWarning() << "Could not open CPUID file at /proc/cpuid";
//    }
//}

//void LinuxCPUStatProvider::updateFrequency()
//{
//    QFile cpuIDFile{ "/proc/cpuinfo" };
//    double cumulated = 0.0;
//    if( cpuIDFile.exists() && cpuIDFile.open( QFile::ReadOnly )) {
//        QTextStream reader{ &cpuIDFile };
//        auto coreIndex = 0;
//        auto line = reader.readLine();
//        while( ! line.isNull() ) {
//            auto comps = line.split( ":" );
//            if( comps.size() >= 2 )  {
//                auto key = comps.at( 0 ).trimmed();
//                auto value = comps.at( 1 ).trimmed();
//                if( key == "cpu MHz" ) {
//                    std::lock_guard< std::mutex > guard( m_lock );
//                    auto freq = value.toDouble();
//                    if( coreIndex == m_coreFrequencies.size() ) {
//                        m_coreFrequencies.push_back( freq );
//                    }
//                    else {
//                        m_coreFrequencies[ coreIndex ] = freq;
//                    }
//                    cumulated += freq;
//                    ++ coreIndex;
//                }
//            }
//            line = reader.readLine();
//        }
//        {
//            std::lock_guard< std::mutex > guard( m_lock );
//            m_avgFrequency = cumulated / m_coreFrequencies.size();
//        }
//    } else {
//        qWarning() << "Could not open CPUID file at /proc/cpuid";
//    }
//}

//void LinuxCPUStatProvider::updateTemparature()
//{
//    //We should check which hwmon%d is for cpu by checking if the symlink
//    //points to a path that contains *coretemp*, for now its not done
//    const QString basePath{ "/sys/class/hwmon/hwmon1" };
//    auto readContent = [ & ]( const QString &fileName ) -> QString {
//        QString content;
//        QFile file{ basePath + "/" + fileName };
//        if( file.exists() && file.open( QFile::ReadOnly )) {
//            content = QString{ file.readAll() };
//        }
//        return content;
//    };

//    int i = 1;
//    int coreIndex = 0;
//    bool physicalDone = false;
//    while( true ) {
//        QString tempLabel;
//        QString tempInput;
//        tempLabel.sprintf( "temp%d_label", i );
//        tempInput.sprintf( "temp%d_input", i );
//        auto label = readContent( tempLabel );
//        auto input = readContent( tempInput ).toInt();
//        auto temp = ( static_cast< double >( input )) / 1000;
//        if( label.isEmpty() ) {
//            break;
//        }
//        else if( label.startsWith( "Physical id" )) {
//            if( physicalDone ) {
//                //right now we support only one processor
//                break;
//            }
//            std::lock_guard< std::mutex > guard( m_lock );
//            m_avgTemp = temp;
//        }
//        else {
//            std::lock_guard< std::mutex > guard( m_lock );
//            if( coreIndex < m_coreTemps.size() ) {
//                m_coreTemps[ coreIndex ] = temp;
//            }
//            else {
//                m_coreTemps.push_back( temp );
//            }
//            ++ coreIndex;
//        }
//        ++ i;
//    }
//}

//void LinuxCPUStatProvider::updateCPUUsage()
//{
////    from: http://stackoverflow.com/a/28063368
////    user nice system idle iowait irq softirq  steal  guest  guest_nice
////    Idle = idle + iowait
////    NonIdle=user+nice+system+irq+softirq+steal
////    Total=Idle+NonIdle # first line of file for all cpus
////    CPU_Percentage=((Total-PrevTotal)-(Idle-PrevIdle))/(Total-PrevTotal)

//    auto calc = [ & ]( int coreIndex, QString &line ) {
////        auto comps = line.split( "")
//        QTextStream stream{ &line };
//        QString what;
//        double user = 0.0;
//        double nice = 0.0;
//        double system = 0.0;
//        double idle = 0.0;
//        double iowait = 0.0;
//        double irq = 0.0;
//        double softirq = 0.0;
//        double steal = 0.0;
//        stream  >> what
//                >> user
//                >> nice
//                >> system
//                >> idle
//                >> iowait
//                >> irq
//                >> softirq
//                >> steal;
//        auto nonIdle = user + nice + system + irq + softirq + steal;
//        auto totalIdle = idle + iowait;
//        auto total = idle + nonIdle;
//        auto cur= &m_avgUsage;
//        if( coreIndex != -1 ) {
//            if( coreIndex == m_coreUsage.size() ) {
//                m_coreUsage.push_back( CPUUsage{ });
//            }
//            cur = &m_coreUsage[ coreIndex ];
//        }
//        auto pc = (( total - cur->m_total ) - ( totalIdle - cur->m_idle ))
//                / ( total - cur->m_total );
//        std::lock_guard< std::mutex > guard( m_lock );
//        cur->m_used = pc * 100;
//        cur->m_total = total;
//        cur->m_idle = totalIdle;
//    };
//    QFile statFile{ "/proc/stat" };
//    auto isFirst = true;
//    auto coreIndex = 0;
//    if( statFile.exists() && statFile.open( QFile::ReadOnly )) {
//        QTextStream stream{ &statFile };
//        auto line = stream.readLine();
//        while( ! line.isNull() ) {
//            if( isFirst && line.startsWith( "cpu" )) {
//                calc( -1, line );
//                isFirst = false;
//            }
//            else if( line.startsWith( "cpu" )) {
//                calc( coreIndex, line );
//                ++ coreIndex;
//            }
//            else {
//                break;
//            }
//            line = stream.readLine();
//        }
//    }
//}

//void LinuxCPUStatProvider::refresh()
//{
//    while( m_keepRunning ) {
//        std::this_thread::sleep_for( std::chrono::milliseconds{ 1000 });
//        updateFrequency();
//        updateTemparature();
//        updateCPUUsage();
//    }
//}

}
