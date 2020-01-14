#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T20:29:25
#
#-------------------------------------------------

QT       += quick sql

TARGET = utilities
TEMPLATE = lib

DEFINES += UTILITIES_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += libzmq-v141-mt-4_3_2.lib
INCLUDEPATH += ../ThirdPartyLib/zmq

SOURCES += \
        Worker/worker.cpp \
        commonmethod.cpp \
        configManager/configarray.cpp \
        configManager/configbase.cpp \
        configManager/configfile.cpp \
        configManager/configmanager.cpp \
        configManager/configobject.cpp \
        configManager/configobjectarray.cpp \
        errorHandling/actionerror.cpp \
        errorHandling/errorhandlingunittest.cpp \
        errorHandling/pickdutfromjigerror.cpp \
        errorHandling/picktrayduterror.cpp \
        errorHandling/placedut2jigerror.cpp \
        errorHandling/placedut2trayerror.cpp \
        errorHandling/silicolerror.cpp \
        loging/LogCollector.cpp \
        loging/LogToFileHandler.cpp \
        loging/Logger.cpp \
        loging/logmodel.cpp \
        loging/logtoviewerhandler.cpp \
        loging/rollbackfile.cpp \
        uiHelper/msgboxmodel.cpp \
        uiHelper/uioperation.cpp \
        userManagement/mysqltablemodel.cpp \
        userManagement/usermanagement.cpp

HEADERS += \
        Worker/worker.h \
        commonmethod.h \
        configManager/configarray.h \
        configManager/configbase.h \
        configManager/configfile.h \
        configManager/configmanager.h \
        configManager/configobject.h \
        configManager/configobjectarray.h \
        configManager/qobjectfactory.h \
        errorHandling/actionerror.h \
        errorHandling/errorhandlingoption.h \
        errorHandling/errorhandlingunittest.h \
        errorHandling/pickdutfromjigerror.h \
        errorHandling/picktrayduterror.h \
        errorHandling/placedut2jigerror.h \
        errorHandling/placedut2trayerror.h \
        errorHandling/silicolerror.h \
        languageManager/languageconfig.h \
        languageManager/languagemanager.h \
        loghelper.h \
        loging/LogBuffer.h \
        loging/LogCollector.h \
        loging/LogToFileHandler.h \
        loging/Logger.h \
        loging/LoggerConfig.h \
        loging/logcollectorconfig.h \
        loging/logmodel.h \
        loging/logtoviewerhandler.h \
        loging/rollbackfile.h \
        loging/utility.h \
        uiHelper/msgboxmodel.h \
        uiHelper/uioperation.h \
        userManagement/mysqltablemodel.h \
        userManagement/usermanagement.h \
        utilities_global.h  \
        zmqWrapper/publisher.h \
        zmqWrapper/subscriber.h



CONFIG(debug, debug|release){
    DESTDIR = ./Debug
    OBJECTS_DIR = ./Debug/Objs
    MOC_DIR = ./Debug/Mocs
}else{
    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES +=

RC_FILE += utilities_resource.rc
