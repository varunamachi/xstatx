#include <QFile>
#include <QDebug>

#include "StatProvider.h"

StatProvider::StatProvider(QObject *parent) : QObject(parent)
{

}

float StatProvider::cpuTemp()
{
    QFile procTemp( "/sys/class/thermal/thermal_zone0/temp" );
    procTemp.open( QFile::ReadOnly );
    auto ba = procTemp.readAll().trimmed();
    bool ok;
    auto temp = ba.toInt( &ok ) / 1000;
//    qDebug() << ba;
    return temp;
}
