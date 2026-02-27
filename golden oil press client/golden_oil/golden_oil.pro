QT+=sql
QT += core gui sql
QT += core gui sql widgets
greaterThan(QT_MAJOR_VERSION,6):QT+=widgets
CONFIG += c++17
SOURCES += \
    connection.cpp \
    main.cpp \
    client.cpp

HEADERS += \
    connection.h \
    client.h

FORMS += \
    client.ui

DISTFILES += \
    connection.sql
qnx:target.path=/tmp/$${TARGET}/bin
else:unix:!android:target.path=/opt/$${TARGET}/bin
!isEmpty(target.path):INSTALLS+=target
