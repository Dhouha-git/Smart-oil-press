QT += core gui sql printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += charts
QT += multimedia multimediawidgets

# serialport : inclus seulement si le module est disponible
qtHaveModule(serialport) {
    QT += serialport
} else {
    message("Qt SerialPort module not found — Arduino features disabled")
    DEFINES += NO_SERIALPORT
}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agriculteur.cpp \
    arduino.cpp \
    client.cpp \
    connection.cpp \
    employe.cpp \
    main.cpp \
    mainwindow.cpp \
    production.cpp \
    vente.cpp

HEADERS += \
    agriculteur.h \
    arduino.h \
    client.h \
    connection.h \
    employe.h \
    mainwindow.h \
    production.h \
    vente.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

