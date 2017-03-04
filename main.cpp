#include <QApplication>
#include <QQmlApplicationEngine>

#include "StatProvider.h"
#include "CPUInfo.h"
#ifdef Q_OS_LINUX
    #include "LinuxCPUStatProvider.h"
#else
    #include "DummyCPUStatProvider.h"
#endif

static QObject *statProviderInstance( QQmlEngine */*engine*/,
                                      QJSEngine */*scriptEngine*/ )
{
    using namespace XStatx;
    auto cpuStatProvider = std::unique_ptr< AbstractCPUStatProvider >{
#ifdef Q_OS_LINUX
        new LinuxCPUStatProvider{}
#else
        new DummyCPUStatProvider{}
#endif
    };
    return new StatProvider{ std::move( cpuStatProvider )};
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    using namespace XStatx;
    qmlRegisterSingletonType< StatProvider >( "vam",
                                              1,
                                              0,
                                              "Stat",
                                              &statProviderInstance );
    qmlRegisterType< CPUInfo >( "vam", 1, 0, "CPUInfo" );
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();

}
