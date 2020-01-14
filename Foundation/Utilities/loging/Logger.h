//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Logger.h
//  @ Date : 2020/1/7
//  @ Author :
//
//

#if !defined(LOGGER_H)
#define LOGGER_H

#include "../commonmethod.h"
#include "../configManager/qobjectfactory.h"
#include "../utilities_global.h"
#include "../zmqWrapper/publisher.h"
#include "../zmqWrapper/subscriber.h"
#include "LoggerConfig.h"
#include "functional"
#include "utility.h"
#include <QDateTime>

class UTILITIESSHARED_EXPORT Logger : public QObject
{
    Q_OBJECT

private:
    Logger();

public:
    static Logger *getIns()
    {
        static Logger instance;
        return &instance;
    }

    void registerCategory(QLoggingCategory *category)
    {
        logCategorys[category->categoryName()] = category;
    }

    void init(bool outputLogToLocalConsole = false);

    void msgHandler(QJsonObject jsonObj);

    static void qlogMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg);

    ~Logger();

private:
    void setLogLevel(QString category, int logLevel)
    {
        if (logCategorys.contains(category))
        {
            setLogEnable(*logCategorys[category], logLevel);
            logCategoryConfigs[category]->setLogLevel(logLevel);
        }
    }

    void setLogEnable(QLoggingCategory &category, int logLevel)
    {
        setLogEnable(category, QtDebugMsg, logLevel);
        setLogEnable(category, QtInfoMsg, logLevel);
        setLogEnable(category, QtWarningMsg, logLevel);
        setLogEnable(category, QtCriticalMsg, logLevel);
    }

    void setLogEnable(QLoggingCategory &category, QtMsgType qmsgType, int logLevel)
    {
        category.setEnabled(qmsgType, qmsgTypeToLevel[qmsgType] >= logLevel);
    }

private:
    bool outputLogToLocalConsole = false;
    LoggerConfig *loggerConfig = nullptr;
    ConfigFile *logConfigFile = nullptr;
    QMap<QString, QLoggingCategory *> logCategorys;
    QMap<QString, LogCategoryConfig *> logCategoryConfigs;
    QMap<QtMsgType, int> qmsgTypeToLevel;
    Publisher logPublisher;
    Subscriber msgSubscriber;
    ConsoleOutputer consoleOutputer;
};

class CategoryRegister
{
public:
    CategoryRegister(QLoggingCategory &category)
    {
        Logger::getIns()->registerCategory(&category);
    }
};

#define SILICOOL_CREATE_LOGGING_CATEGORY(categoryGetterName, categoryName)                                             \
    inline QLoggingCategory &categoryGetterName()                                                                      \
    {                                                                                                                  \
        static QLoggingCategory category(categoryName);                                                                \
        return category;                                                                                               \
    }                                                                                                                  \
    static CategoryRegister categoryGetterName##___(categoryGetterName());

#define SILICOOL_DECLARE_LOGGING_CATEGORY(name) extern QLoggingCategory &name();

#endif    // LOGGER_H
