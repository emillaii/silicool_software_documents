#ifndef LENSLOADER_H
#define LENSLOADER_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

SILICOOL_CREATE_LOGGING_CATEGORY(lensLoader, "LensLoader")
SILICOOL_CREATE_LOGGING_CATEGORY(lensPickArm, "LensPickArm")

class LensLoader : public QObject
{
    Q_OBJECT

public:
    LensLoader() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~LensLoader()
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
        qCDebug(lensLoader) << "LensLoader"
                            << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sdf你好");
        qCInfo(lensLoader) << "LensLoader"
                           << "timer id:" << event->timerId() << getCurrentDateTime() << QString("sfsdfas天啊");
        qCWarning(lensLoader) << "LensLoader"
                              << "timer id:" << event->timerId() << getCurrentDateTime()
                              << QString("sdfsafsadfsdafsdf哪个");
        qCCritical(lensLoader) << "LensLoader"
                               << "timer id:" << event->timerId() << getCurrentDateTime() << QString("是呢sdf");
    }
};

#endif    // LENSLOADER_H
