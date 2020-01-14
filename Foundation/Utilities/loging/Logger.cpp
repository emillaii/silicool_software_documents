#include "Logger.h"
#include <iostream>

Logger::Logger() : logPublisher("!@#$%"), msgSubscriber(nullptr, "!@#$%")
{
    qmsgTypeToLevel[QtDebugMsg] = LogLevel::Debug;
    qmsgTypeToLevel[QtInfoMsg] = LogLevel::Info;
    qmsgTypeToLevel[QtWarningMsg] = LogLevel::Warning;
    qmsgTypeToLevel[QtCriticalMsg] = LogLevel::Critical;
    qmsgTypeToLevel[QtFatalMsg] = LogLevel::Fatal;

    registerCategory(QLoggingCategory::defaultCategory());
}

void Logger::init(bool outputLogToLocalConsole)
{
    this->outputLogToLocalConsole = outputLogToLocalConsole;
    loggerConfig = new LoggerConfig;
    logConfigFile = new ConfigFile("logConfig", loggerConfig, "logConfig.json", false);
    if (!logConfigFile->populate(true))
    {
        qFatal("Please set %s correctly first!", logConfigFile->getFileName().toStdString().c_str());
    }
    logConfigFile->setAutoSave(false);
    QList<int> unUsedCategory;
    auto categoryConfigs = loggerConfig->categoryConfigs();
    for (int i = 0; i < categoryConfigs->count(); i++)
    {
        auto categoryConfig = dynamic_cast<LogCategoryConfig *>(categoryConfigs->at(i));
        if (logCategorys.contains(categoryConfig->category()))
        {
            logCategoryConfigs[categoryConfig->category()] = categoryConfig;
            if (categoryConfig->logLevel() >= LogLevel::Debug && categoryConfig->logLevel() < LogLevel::Fatal)
            {
                setLogEnable(*logCategorys[categoryConfig->category()], categoryConfig->logLevel());
            }
        }
        else
        {
            unUsedCategory.append(i);
        }
    }
    for (int i = unUsedCategory.count() - 1; i >= 0; i--)
    {
        categoryConfigs->remove(unUsedCategory[i]);
    }
    foreach (auto registeredCategory, logCategorys.keys())
    {
        if (!logCategoryConfigs.contains(registeredCategory))
        {
            categoryConfigs->add(0);
            auto newCategoryConfig = dynamic_cast<LogCategoryConfig *>(categoryConfigs->at(0));
            newCategoryConfig->setCategory(registeredCategory);
            logCategoryConfigs[registeredCategory] = newCategoryConfig;
        }
    }
    logConfigFile->save();
    logConfigFile->setAutoSave(true);

    try
    {
        logPublisher.bind(loggerConfig->logPublishAddr());
        msgSubscriber.subscribe(loggerConfig->msgSubscribeAddr());
    }
    catch (std::exception &ex)
    {
        qFatal(ex.what());
    }
    msgSubscriber.installMessageHandle(std::bind(&Logger::msgHandler, this, std::placeholders::_1));
    msgSubscriber.startListenMessage();

    qSetMessagePattern("%{function}");
    qInstallMessageHandler(qlogMessageHandler);
}

void Logger::msgHandler(QJsonObject jsonObj)
{
    if (jsonObj["msgName"].toString() == QString("setLogLevel"))
    {
        setLogLevel(jsonObj["category"].toString(), jsonObj["level"].toInt());
    }
}

void Logger::qlogMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    QJsonObject jsonObj;
    auto ins = getIns();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    jsonObj["time"] = time;
    jsonObj["category"] = context.category;
    int logLevel = ins->qmsgTypeToLevel[msgType];
    jsonObj["level"] = logLevel;
    auto souceCodeFileName = QFileInfo(context.file).fileName();
    jsonObj["location"]
        = QString("%1:%2(%3)").arg(souceCodeFileName).arg(context.line).arg(qFormatLogMessage(msgType, context, msg));
    jsonObj["msg"] = msg;
    ins->logPublisher.sendJson(jsonObj);
    if (ins->outputLogToLocalConsole)
    {
        QString postfixMsg = QString("[%1] %2:%3")
                                 .arg(LogLevel::getIns().toString(logLevel))
                                 .arg(jsonObj["location"].toString())
                                 .arg(jsonObj["msg"].toString());
        QString formatedMsg = QString("%1 [%2] %3").arg(time).arg(context.category).arg(postfixMsg);
        ins->consoleOutputer.outputLogToConsole(logLevel, formatedMsg);
    }
}

Logger::~Logger()
{
    msgSubscriber.stopListenMessage();
    qInstallMessageHandler(nullptr);
    SILICOOL_DELETE(logConfigFile);
    SILICOOL_DELETE(loggerConfig);
}
