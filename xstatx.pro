TEMPLATE = app

QT +=  qml quick charts
CONFIG += c++11

SOURCES += main.cpp \
    StatProvider.cpp
    
HEADERS += \
    StatProvider.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


