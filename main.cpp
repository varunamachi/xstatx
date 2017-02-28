#include <QApplication>
#include <QQmlApplicationEngine>

#include "StatProvider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    using namespace XStatx;
    qmlRegisterSingletonType< StatProvider >( "vam",
                                              1,
                                              0,
                                              "Stat",
                                              &StatProvider::qmlInstance);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
