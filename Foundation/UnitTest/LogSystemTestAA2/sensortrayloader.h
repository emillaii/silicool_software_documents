#ifndef SENSORTRAYLOADER_H
#define SENSORTRAYLOADER_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

class SensorTrayLoader : public QObject
{
    Q_OBJECT

public:
    SensorTrayLoader() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~SensorTrayLoader()
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
        qDebug() << "SensorTrayLoader"
                 << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sdf你好");
        qInfo() << "SensorTrayLoader"
                << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sfsdfas天啊");
        qWarning() << "SensorTrayLoader"
                   << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sdfsafsadfsdafsdf哪个");
        qCritical() << "SensorTrayLoader"
                    << "timer id:" << event->timerId() << getCurrentDateTime() << QString("是呢sdf");
    }
};

#endif    // SENSORTRAYLOADER_H
