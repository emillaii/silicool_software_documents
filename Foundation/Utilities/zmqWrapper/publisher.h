#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "zmq.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

class UTILITIESSHARED_EXPORT Publisher
{
public:
    Publisher(QString msgEndMark, QString msgPrefix = "")
    {
        this->msgPrefix = msgPrefix.toUtf8();
        this->msgEndMark = msgEndMark.toUtf8();
        ctx = new zmq::context_t;
        publisher = new zmq::socket_t(*ctx, zmq::socket_type::pub);
    }

    void bind(QString address)
    {
        try
        {
            publisher->bind(address.toStdString());
        }
        catch (std::exception &ex)
        {
            throw std::exception(QString("Zmq bind to address error! Error message: %1, address: %2")
                                     .arg(ex.what())
                                     .arg(address)
                                     .toStdString()
                                     .c_str());
        }
    }

    void sendJson(const QJsonObject &jsonObj)
    {
        auto buffer = QJsonDocument(jsonObj).toBinaryData() + msgEndMark;
        if (!msgPrefix.isEmpty())
        {
            buffer = msgPrefix + buffer;
        }
        size_t nPublishedBytes = 0;
        size_t bufferLen = buffer.length();
        char *data = buffer.data();
        QMutexLocker tmpLocker(&locker);
        while (nPublishedBytes < bufferLen)
        {
            nPublishedBytes += publisher->send(data + nPublishedBytes, bufferLen - nPublishedBytes);
        }
    }

    ~Publisher()
    {
        SILICOOL_DELETE(publisher);
        SILICOOL_DELETE(ctx);
    }

private:
    void sendBytes(const void *buffer, size_t length) {}

private:
    QByteArray msgPrefix;
    QByteArray msgEndMark;
    QMutex locker;
    zmq::context_t *ctx = nullptr;
    zmq::socket_t *publisher = nullptr;
};

#endif    // PUBLISHER_H
