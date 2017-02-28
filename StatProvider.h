#pragma once

#include <QObject>

class QQmlEngine;
class QJSEngine;

namespace XStatx {

class StatProvider : public QObject
{
    Q_OBJECT
public:
    explicit StatProvider(QObject *parent = 0);

    Q_INVOKABLE float cpuTemp();

    static QObject *qmlInstance( QQmlEngine */*engine*/,
                                 QJSEngine */*scriptEngine*/ )
    {
        return new StatProvider{};
    }

signals:

public slots:
};


}
