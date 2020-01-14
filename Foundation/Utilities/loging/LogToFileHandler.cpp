#include "LogToFileHandler.h"

LogToFileHandler::LogToFileHandler(bool outputToConsole) : outputToConsole(outputToConsole)
{
    config = new LogCollectorConfig;
    configFile = new ConfigFile("logCollectorConfig", config, "logCollectorConfig.json", false);
}

void LogToFileHandler::init()
{
    QString errMsg = QString("Please set %1 correctly first!").arg(configFile->getFileName());
    if (!configFile->populate(true))
    {
        qFatal(errMsg.toStdString().c_str());
    }
    if (config->logDir().isEmpty())
    {
        qFatal(errMsg.toStdString().c_str());
    }
    LogCollector::init(config->addrToBeListened());
}

LogToFileHandler::~LogToFileHandler()
{
    logSubscriber.stopListenMessage();
    SILICOOL_DELETE(configFile);
    SILICOOL_DELETE(config);
    foreach (auto rollbackFile, rollbackFiles.values())
    {
        delete rollbackFile;
    }
}

void LogToFileHandler::onUnCategorizedLog(int logLevel, QString &msg)
{
    if (!rollbackFiles.contains(defaultCategoryName))
    {
        addRollbackFile(defaultCategoryName, "silicoolLog");
    }
    rollbackFiles[defaultCategoryName]->appendLine(msg);
    if (logLevel >= LogLevel::Warning)
    {
        if (!rollbackFiles.contains(warningCategory))
        {
            addRollbackFile(warningCategory, warningLogName);
        }
        rollbackFiles[warningCategory]->appendLine(msg);
    }

    if (outputToConsole)
    {
        consoleOutputer.outputLogToConsole(logLevel, msg);
    }
}

void LogToFileHandler::onCategorizedLog(QString &category, int logLevel, QString &msg)
{
    Q_UNUSED(logLevel)
    if (!rollbackFiles.contains(category))
    {
        addRollbackFile(category, category);
    }
    rollbackFiles[category]->appendLine(msg);
}
