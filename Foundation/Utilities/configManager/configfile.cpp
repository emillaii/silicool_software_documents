#include "configfile.h"

ConfigFile::ConfigFile(QString configBaseName, ConfigBase *configBase, QString fileName, bool encript)
    : configBaseName(configBaseName), configBase(configBase), fileName(fileName), encript(encript)
{
    if (configBase == nullptr)
    {
        qFatal("configBase is null!");
        return;
    }
    bool gotSlotIndex = false;
    for (int i = 0; i < metaObject()->methodCount(); i++)
    {
        auto method = metaObject()->method(i);
        if (method.methodType() == QMetaMethod::Slot && QString(method.name()) == QString("save"))
        {
            m_saveSlotIndex = method.methodIndex();
            gotSlotIndex = true;
            break;
        }
    }
    if (!gotSlotIndex)
    {
        qFatal("Did not find slot! Name: save");
    }
    else
    {
        configBase->connectConfigChangedSignalToSlot(this, m_saveSlotIndex, true);
    }
}

bool ConfigFile::populate(bool createEmptyFileIfFileNotExist)
{
    setAutoSave(false);
    bool result = _populate(createEmptyFileIfFileNotExist);
    setAutoSave(true);
    return result;
}

void ConfigFile::save()
{
    qDebug() << QString("Save config file! File name: %1").arg(fileName);

    QFileInfo fileInfo(fileName);
    QDir dir = fileInfo.dir();
    if (!dir.exists())
    {
        dir.mkpath(fileInfo.path());
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "Open file failed: " << fileName;
        return;
    }
    QJsonObject topJsonObj;
    QJsonValue jsonValue;
    configBase->write(jsonValue);
    topJsonObj[configBaseName] = jsonValue;
    topJsonObj["Timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QJsonDocument newJsonDoc(topJsonObj);
    if (encript)
    {
        file.write(newJsonDoc.toJson().toBase64());
    }
    else
    {
        file.write(newJsonDoc.toJson());
    }
    file.close();
}

bool ConfigFile::_populate(bool createEmptyFileIfFileNotExist)
{
    QFile file(fileName);
    if (!file.exists())
    {
        qCritical() << QString("Did not exist json file: %1").arg(fileName);
        if (createEmptyFileIfFileNotExist)
        {
            save();
            qInfo() << QString("Create an empty config file!");
        }
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << QString("Open json file failed: %1").arg(fileName);
        return false;
    }
    QByteArray fileContent = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromBase64(fileContent));
    if (jsonDoc.isNull() || jsonDoc.isEmpty())
    {
        jsonDoc = QJsonDocument::fromJson(fileContent);
        if (jsonDoc.isNull() || jsonDoc.isEmpty())
        {
            qCritical() << QString("Json file %1 is invalid!").arg(fileName);
            return false;
        }
    }

    if (!jsonDoc.object().contains(configBaseName))
    {
        qCritical() << QString("Did not exist config base named %1 in json file %2.").arg(configBaseName).arg(fileName);
        return false;
    }
    return configBase->read(jsonDoc.object()[configBaseName]);
}
