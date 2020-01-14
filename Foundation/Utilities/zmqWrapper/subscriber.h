#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "zmq.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QThread>

typedef std::function<void(QJsonObject)> JsonMessageHandler;

class UTILITIESSHARED_EXPORT Subscriber : public QThread
{
    Q_OBJECT

public:
    explicit Subscriber(QObject *parent,
                        QString endMark,
                        QString messagePrefix = "",
                        int receiveTimeout = 100,
                        int bufferLen = 1024 * 100)
        : QThread(parent), bufferLen(bufferLen)
    {
        this->endMark = endMark.toUtf8();
        endMarkData = this->endMark.data();
        endMarkDataLen = this->endMark.length();
        buffer = new char[bufferLen];
        memset(buffer, 0, bufferLen);
        ctx = new zmq::context_t;
        subscriber = new zmq::socket_t(*ctx, zmq::socket_type::sub);
        auto msgPrefix = messagePrefix.toUtf8();
        subscriber->setsockopt(ZMQ_SUBSCRIBE, msgPrefix, static_cast<size_t>(msgPrefix.length()));
        subscriber->setsockopt(ZMQ_RCVTIMEO, &receiveTimeout, sizeof(int));
    }

    void subscribe(QString address)
    {
        try
        {
            subscriber->connect(address.toStdString());
        }
        catch (std::exception &ex)
        {
            throw std::exception(QString("Zmq connect to address error! Error message: %1, address: %2")
                                     .arg(ex.what())
                                     .arg(address)
                                     .toStdString()
                                     .c_str());
        }
    }

    void installMessageHandle(JsonMessageHandler msgHandle)
    {
        this->messageHandler = msgHandle;
    }

    void clearBuffer()
    {
        validDataLen = 0;
        lastCompareIndex = -1;
    }

    bool receiveJson(QJsonObject &jsonObj, int maxRetry = 10)
    {
        QMutexLocker tmpLocker(&locker);
        for (int t = 0; t < maxRetry; t++)
        {
            if (validDataLen >= bufferLen)
            {
                qFatal("Did not find end mark: %s, buffer may be too small: %d", endMarkData, bufferLen);
            }
            int receivedSize = subscriber->recv(buffer + validDataLen, bufferLen - validDataLen);
            if (receivedSize > 0)
            {
                validDataLen += receivedSize;
            }
            if (validDataLen > 0)
            {
                for (int i = lastCompareIndex + 1; i <= validDataLen - endMarkDataLen; i++)
                {
                    bool findEndMark = true;
                    for (int j = 0; j < endMarkDataLen; j++)
                    {
                        if (buffer[i + j] != endMarkData[j])
                        {
                            findEndMark = false;
                            break;
                        }
                    }
                    if (findEndMark)
                    {
                        jsonObj = QJsonDocument::fromBinaryData(QByteArray(buffer, i)).object();
                        validDataLen -= (i + endMarkDataLen);
                        if (validDataLen > 0)
                        {
                            memcpy(buffer, buffer + i + endMarkDataLen, validDataLen);
                        }
                        lastCompareIndex = -1;
                        return true;
                    }
                }
                lastCompareIndex = validDataLen - endMarkDataLen;
            }
        }
        return false;
    }

    void startListenMessage()
    {
        if (isRun)
        {
            return;
        }
        if (messageHandler == nullptr)
        {
            qCritical("Please installMessageHandle first!");
            return;
        }
        isRun = true;
        start();
    }

    void stopListenMessage()
    {
        if (isRun)
        {
            isRun = false;
            wait();
        }
    }

    virtual ~Subscriber() override
    {
        stopListenMessage();
        delete[] buffer;
        SILICOOL_DELETE(subscriber);
        SILICOOL_DELETE(ctx);
    }

    // QThread interface
protected:
    virtual void run() override
    {
        QJsonObject jsonObj;
        while (isRun)
        {
            if (receiveJson(jsonObj, 8))
            {
                messageHandler(jsonObj);
            }
        }
    }

private:
    QByteArray endMark;
    char *endMarkData;
    int endMarkDataLen;

    int bufferLen;
    char *buffer;
    int validDataLen = 0;
    int lastCompareIndex = -1;

    bool isRun = false;
    QMutex locker;
    zmq::context_t *ctx = nullptr;
    zmq::socket_t *subscriber = nullptr;
    JsonMessageHandler messageHandler = nullptr;
};
#endif    // SUBSCRIBER_H
