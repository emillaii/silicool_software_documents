#ifndef LOGVIEWERCONFIG_H
#define LOGVIEWERCONFIG_H

#include "../configManager/configarray.h"
#include "../configManager/configfile.h"
#include "../utilities_global.h"

class UTILITIESSHARED_EXPORT LogCollectorConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString logDir READ logDir WRITE setLogDir NOTIFY logDirChanged)
    Q_PROPERTY(QString msgPublishAddr READ msgPublishAddr WRITE setMsgPublishAddr NOTIFY msgPublishAddrChanged)
    Q_PROPERTY(ConfigArray *addrToBeListened READ addrToBeListened)

    QString m_msgPublishAddr;

    ConfigArray *m_addrToBeListened;

    QString m_logDir;

public:
    LogCollectorConfig()
    {
        m_addrToBeListened = new ConfigArray(ConfigElementInfo::Other);
        init();
    }

    QString msgPublishAddr() const
    {
        return m_msgPublishAddr;
    }
    ConfigArray *addrToBeListened() const
    {
        return m_addrToBeListened;
    }

    QString logDir() const
    {
        return m_logDir;
    }

public slots:
    void setMsgPublishAddr(QString msgPublishAddr)
    {
        if (m_msgPublishAddr == msgPublishAddr)
            return;

        m_msgPublishAddr = msgPublishAddr;
        emit msgPublishAddrChanged(m_msgPublishAddr);
    }

    void setLogDir(QString logDir)
    {
        if (m_logDir == logDir)
            return;

        m_logDir = logDir;
        emit logDirChanged(m_logDir);
    }

signals:
    void msgPublishAddrChanged(QString msgPublishAddr);
    void logDirChanged(QString logDir);
};

#endif    // LOGVIEWERCONFIG_H
