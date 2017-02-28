#include <QFile>
#include <QDebug>
#include <QDateTime>

#include "StatProvider.h"

namespace XStatx {

StatProvider::StatProvider(QObject *parent) : QObject(parent)
{
    qsrand( static_cast< uint >( QTime::currentTime().msec() ));
}

float StatProvider::cpuTemp()
{
#ifdef Q_OS_LINUX
    QFile procTemp( "/sys/class/thermal/thermal_zone0/temp" );
    procTemp.open( QFile::ReadOnly );
    auto ba = procTemp.readAll().trimmed();
    bool ok;
    auto temp = ba.toInt( &ok ) / 1000;
//    qDebug() << ba;
    return temp;
#else
    return qrand() % 50 + 30;
#endif
}

}
