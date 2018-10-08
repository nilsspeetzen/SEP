#-------------------------------------------------
#
# Project created by QtCreator 2018-08-07T20:13:32
#
#-------------------------------------------------

QT       += qml
CONFIG   += c++1z

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlaKas
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    "src/Eigen3/eigen3" \
    "src/Eigen3/eigen3/unsupported" \
    "src/Eigen3/eigen3/unsupported/Eigen/src/NonLinearOptimization" \
    "src/DcoCppCe/include" \
    "src/Eso/include" \
    "src/Yasp/include" \
    "src/BlockDeco/include" \
    "src/BlockDecoBTF/include"

SOURCES += \
        main.cpp

HEADERS += \
    flash.h \
    module.h \
    lin_sys.h \
    cascade.h \
    WorldMachine.h \
    cascadeeso.h

FORMS +=

DISTFILES += \
    main.qml \
    flash.qml \
    dataWindow.qml

RESOURCES += \
    qml.qrc
