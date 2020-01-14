#ifndef TRAYLOADER_H
#define TRAYLOADER_H

#include "Logger.h"
#include "commonmethod.h"
#include <QObject>

class TrayLoader : public QObject
{
    Q_OBJECT

public:
    TrayLoader() {}

    Q_INVOKABLE void startWriteLog()
    {
        timerIDs.append(startTimer(100));
    }

    ~TrayLoader()
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
        print(event->timerId());
    }

private:
    void print(int id)
    {
        qDebug() << "TrayLoader"
                 << "timer id:" << id << getCurrentDateTime() << QString("sdf你好");
        qInfo() << "TrayLoader"
                << "timer id:" << id << getCurrentDateTime() << QString("sfsdfas天啊");
        qWarning() << "TrayLoader"
                   << "timer id:" << id << getCurrentDateTime() << QString("sdfsafsadfsdafsdf哪个");
        qCritical() << "TrayLoader"
                    << "timer id:" << id << getCurrentDateTime() << QString("是呢sdf");
    }
};

#endif    // TRAYLOADER_H
