#-------------------------------------------------
#
# Project created by QtCreator 2017-12-16T13:48:54
#
#-------------------------------------------------

#QT       += core gui serialport
greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
<<<<<<< HEAD
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
=======
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt5support/serialport.prf)
>>>>>>> version on raspbi w/ gpio
}
TARGET = meritve
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    masterthread.cpp \
<<<<<<< HEAD
    calibration.cpp

HEADERS  += mainwindow.h \
    masterthread.h \
    calibration.h
=======
    calibration.cpp \
    mmapGpio.cpp \
    gpiothread.cpp

HEADERS  += mainwindow.h \
    masterthread.h \
    calibration.h \
    mmapGpio.h \
    gpiothread.h
>>>>>>> version on raspbi w/ gpio

FORMS    += mainwindow.ui
