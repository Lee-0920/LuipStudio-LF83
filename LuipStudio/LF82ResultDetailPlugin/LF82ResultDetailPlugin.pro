#-------------------------------------------------
#
# Project created by QtCreator 2017-07-24T14:01:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = LF82ResultDetailPlugin
TEMPLATE = lib

DESTDIR = ../../bin

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../Luip ../
INCLUDEPATH += ../Lib/log4cpp/include
INCLUDEPATH += ../Lib/qwt/include
INCLUDEPATH += ../Lib/lua/include
INCLUDEPATH += ../Lib/oolua/include

DEFINES += OOLUA_USE_EXCEPTIONS=1 OOLUA_STORE_LAST_ERROR=0

win32 {
    DEFINES += _CS_X86_WINDOWS
    DEFINES += BIL_EXPORT

    Debug:LIBS   += -L$$PWD/../Lib/qwt/lib/windows/ -lqwtd \
                    -L$$PWD/../Lib/oolua/lib/windows/ -looluaD

    Release:LIBS += -L$$PWD/../Lib/qwt/lib/windows/ -lqwt \
                    -L$$PWD/../Lib/oolua/lib/windows/ -loolua

    LIBS   +=  -L$$PWD/../Lib/log4cpp/lib/windows/ -llog4cpp \
               -L$$PWD/../Lib/lua/lib/windows/ -llua \
               -L$$PWD/../../bin/ -lLuaEngine \
               -L$$PWD/../../bin/ -lRecordDataPlugin \
               -L$$PWD/../../bin/ -lResultDetailPlugin \
               -L$$PWD/../../bin/ -lControllerPlugin
}

unix {
     CONFIG += arm
#    CONFIG += x86

    x86 {
        DEFINES += _CS_X86_LINUX
        LIBS += -L$$PWD/../Lib/qwt/lib/linux-x86 -lqwt \
                -L$$PWD/../Lib/log4cpp/lib/linux-x86 -llog4cpp \
                -L$$PWD/../Lib/lua/lib/linux-x86 -llua \
                -L$$PWD/../Lib/oolua/lib/linux-x86 -loolua \
                -L$$PWD/../../bin/ -lLuaEngine \
                -L$$PWD/../../bin/ -lControllerPlugin \
                -L$$PWD/../../bin/ -lRecordDataPlugin \
                -L$$PWD/../../bin/ -lResultDetailPlugin
    }
    arm {

        DEFINES += _CS_ARM_LINUX
        LIBS += -L$$PWD/../Lib/qwt/lib/linux-arm -lqwt \
                -L$$PWD/../Lib/log4cpp/lib/linux-arm -llog4cpp \
                -L$$PWD/../Lib/lua/lib/linux-arm -llua \
                -L$$PWD/../Lib/oolua/lib/linux-arm -loolua \
                -L$$PWD/../../bin/ -lLuaEngine \
                -L$$PWD/../../bin/ -lControllerPlugin \
                -L$$PWD/../../bin/ -lRecordDataPlugin \
                -L$$PWD/../../bin/ -lResultDetailPlugin
    }
}

android {
        CONFIG += android

        DEFINES += _CS_ARM_LINUX
        DEFINES += _CS_ANDROID
        LIBS += -L$$PWD/../Lib/log4cpp/lib/android -llog4cpp-larm \
                -L$$PWD/../Lib/qwt/lib/android -lqwt
}

SOURCES += \
    LF82ResultDetailPlugin.cpp \
    Log.cpp \
    LF82ResultDetailWidget.cpp \
    LF82ResultDetailPluginProxy.cpp \
    LF82CurveData.cpp \
    LF82CurveFile.cpp \
    LF82CurveManager.cpp

HEADERS += \
    LF82ResultDetailPlugin.h \
    Log.h \
    LF82ResultDetailWidget.h \
    LF82ResultDetailPluginProxy.h \
    LF82CurveData.h \
    LF82CurveFile.h \
    LF82CurveManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
