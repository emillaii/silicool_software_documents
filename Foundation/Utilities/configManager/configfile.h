#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "../utilities_global.h"
#include "configobject.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QObject>

class UTILITIESSHARED_EXPORT ConfigFile : public QObject
{
    Q_OBJECT

public:
    ConfigFile(QString configBaseName, ConfigBase *configBase, QString fileName, bool encript = true);

    QString getFileName() const
    {
        return fileName;
    }

    bool populate(bool createEmptyFileIfFileNotExist = false);

    void setAutoSave(bool autoSave)
    {
        configBase->connectConfigChangedSignalToSlot(this, m_saveSlotIndex, autoSave);
    }

public slots:
    void save();

private:
    bool _populate(bool createEmptyFileIfFileNotExist);

private:
    QString configBaseName;
    ConfigBase *configBase = nullptr;
    QString fileName;
    bool encript;
    int m_saveSlotIndex;
};

#endif    // CONFIGFILE_H
