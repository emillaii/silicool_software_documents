#include "logtoviewerhandler.h"

LogToViewerHandler::LogToViewerHandler() : msgPublisher("!@#$%")
{
    levelToColor[0] = "white";
    levelToColor[1] = "white";
    levelToColor[2] = "yellow";
    levelToColor[3] = "tomato";
    levelToColor[4] = "tomato";

    config = new LogCollectorConfig;
    configFile = new ConfigFile("logCollectorConfig", config, "logCollectorConfig.json", false);

    connect(this, &LogToViewerHandler::appendCategorySignal, this, &LogToViewerHandler::appendCategory,
            Qt::BlockingQueuedConnection);
    appendCategory(defaultCategoryName);
}

void LogToViewerHandler::init()
{
    QString errMsg = QString("Please set %1 correctly first!").arg(configFile->getFileName());
    if (!configFile->populate(true))
    {
        qFatal(errMsg.toStdString().c_str());
    }
    LogCollector::init(config->addrToBeListened());
    detectLogBufferTimerID = startTimer(DetectLogBufferInterval);
    try
    {
        msgPublisher.bind(config->msgPublishAddr());
    }
    catch (std::exception &ex)
    {
        qFatal(ex.what());
    }
}

LogToViewerHandler::~LogToViewerHandler()
{
    logSubscriber.stopListenMessage();
    if (detectLogBufferTimerID != -1)
    {
        killTimer(detectLogBufferTimerID);
        detectLogBufferTimerID = -1;
    }
    SILICOOL_DELETE(configFile);
    SILICOOL_DELETE(config);
    foreach (auto logBuffer, logBuffers.values())
    {
        delete logBuffer;
    }
    foreach (auto logModel, logModels.values())
    {
        delete logModel;
    }
}
