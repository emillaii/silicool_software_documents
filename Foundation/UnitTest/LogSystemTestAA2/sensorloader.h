#ifndef SENSORLOADER_H
#define SENSORLOADER_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

SILICOOL_CREATE_LOGGING_CATEGORY(sensorLoaderCate, "SensorLoader")
SILICOOL_CREATE_LOGGING_CATEGORY(sensorPickArmCate, "SensorPickArm")

class SensorLoader : public QObject
{
    Q_OBJECT

public:
    SensorLoader() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~SensorLoader()
    {
        while (timerIDs.count() > 0)
        {
            killTimer(timerIDs[0]);
            timerIDs.removeAt(0);
        }
    }

private:
    QList<int> timerIDs;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override
    {
        qCDebug(sensorLoaderCate()) << "SensorLoader"
                                    << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sdf你好");
        qCInfo(sensorLoaderCate()) << "SensorLoader"
                                   << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sfsdfas天啊");
        qCWarning(sensorLoaderCate()) << "SensorLoader"
                                      << "timer id:" << event->timerId() << getCurrentDateTime()
                                      << QString("sdfsafsadfsdafsdf哪个");
        qCCritical(sensorLoaderCate()) << "SensorLoader"
                                       << "timer id:" << event->timerId() << getCurrentDateTime() << QString("是呢sdf");
    }
};

#endif    // SENSORLOADER_H
