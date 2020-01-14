#ifndef LENSPICKARM_H
#define LENSPICKARM_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

SILICOOL_DECLARE_LOGGING_CATEGORY(lensPickArm)

class LensPickArm : public QObject
{
    Q_OBJECT

public:
    LensPickArm() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~LensPickArm()
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
        qCDebug(lensPickArm) << "lensPickArm"
                             << "timer id:" << event->timerId() << getCurrentDateTime();
        qCInfo(lensPickArm) << "lensPickArm"
                            << "timer id:" << event->timerId() << getCurrentDateTime();
        qCWarning(lensPickArm) << "lensPickArm"
                               << "timer id:" << event->timerId() << getCurrentDateTime();
        qCCritical(lensPickArm) << "lensPickArm"
                                << "timer id:" << event->timerId() << getCurrentDateTime();
    }
};

#endif    // LENSPICKARM_H
