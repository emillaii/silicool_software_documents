#ifndef LOGTOVIEWERHANDLER_H
#define LOGTOVIEWERHANDLER_H

#include "../utilities_global.h"
#include "../zmqWrapper/publisher.h"
#include "LogBuffer.h"
#include "LogCollector.h"
#include "logcollectorconfig.h"
#include "logmodel.h"

class UTILITIESSHARED_EXPORT LogToViewerHandler : public QObject, public LogCollector
{
    Q_OBJECT
public:
    LogToViewerHandler();

    void init();

    ~LogToViewerHandler() override;

    Q_INVOKABLE void setLogLevel(QString category, int level)
    {
        QJsonObject jsonMsg;
        jsonMsg["msgName"] = "setLogLevel";
        jsonMsg["category"] = category;
        jsonMsg["level"] = level;
        msgPublisher.sendJson(jsonMsg);
    }

    Q_INVOKABLE QObject *getLogModel(QString category)
    {
        if (logModels.contains(category))
        {
            return logModels[category];
        }
        else
        {
            qCritical() << "Did not find category:" << category;
            return nullptr;
        }
    }

    Q_INVOKABLE QString getDefaultCategoryName() const
    {
        return defaultCategoryName;
    }

    // LogCollector interface
protected:
    virtual void onUnCategorizedLog(int logLevel, QString &msg) override
    {
        logBuffers[defaultCategoryName]->appendLog(new LogItem{ levelToColor[logLevel], msg });
        if (logLevel >= LogLevel::Warning)
        {
            if (!logBuffers.contains(warnLogName))
            {
                emit appendCategorySignal(warnLogName);
                emit newLogModelAttached(warnLogName);
            }
            logBuffers[warnLogName]->appendLog(new LogItem{ levelToColor[logLevel], msg });
        }
    }

    virtual void onCategorizedLog(QString &category, int logLevel, QString &msg) override
    {
        if (!logBuffers.contains(category))
        {
            emit appendCategorySignal(category);
            emit newLogModelAttached(category);
        }
        logBuffers[category]->appendLog(new LogItem{ levelToColor[logLevel], msg });
    }

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override
    {
        if (event->timerId() == detectLogBufferTimerID)
        {
            emit detectLogBuffer();
        }
    }

private slots:
    void appendCategory(QString category)
    {
        auto logBuffer = new LogBuffer;
        auto logModel = new LogModel;
        connect(this, &LogToViewerHandler::detectLogBuffer, logBuffer, &LogBuffer::onDetectBuffer);
        connect(logBuffer, &LogBuffer::logBufferChanged, logModel, &LogModel::onLogBufferChanged);
        logBuffers[category] = logBuffer;
        logModels[category] = logModel;
    }

signals:
    void appendCategorySignal(QString category);
    void detectLogBuffer();
    void newLogModelAttached(QString category);

private:
    const QString warnLogName = "Warn";
    int detectLogBufferTimerID = -1;
    const int DetectLogBufferInterval = 180;
    QMap<int, QString> levelToColor;
    Publisher msgPublisher;
    QMap<QString, LogBuffer *> logBuffers;
    QMap<QString, LogModel *> logModels;
    LogCollectorConfig *config;
    ConfigFile *configFile;
};

#endif    // LOGTOVIEWERHANDLER_H
