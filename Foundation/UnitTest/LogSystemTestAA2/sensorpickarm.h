#ifndef SENSORPICKARM_H
#define SENSORPICKARM_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

SILICOOL_DECLARE_LOGGING_CATEGORY(sensorPickArmCate)

class SensorPickArm : public QObject
{
    Q_OBJECT

public:
    SensorPickArm() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~SensorPickArm()
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
        qCDebug(sensorPickArmCate()) << "sensorPickArm"
                                     << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sdf你好");
        qCInfo(sensorPickArmCate()) << "sensorPickArm"
                                    << "timer id:" << event->timerId() << getCurrentDateTime()
                                    << QString("sfsdfas天啊");
        qCWarning(sensorPickArmCate()) << "sensorPickArm"
                                       << "timer id:" << event->timerId() << getCurrentDateTime()
                                       << QString("sdfsafsadfsdafsdf哪个");
        qCCritical(sensorPickArmCate()) << "sensorPickArm"
                                        << "timer id:" << event->timerId() << getCurrentDateTime()
                                        << QString("是呢sdf");
    }
};

#endif    // SENSORPICKARM_H
