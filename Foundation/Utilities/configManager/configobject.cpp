#include "configobject.h"

SetObjectItemCommand::SetObjectItemCommand(ConfigObject *target,
                                           QString &propName,
                                           QVariant &oldValue,
                                           QVariant &newValue)
    : target(target), propName(propName), oldValue(oldValue), newValue(newValue)
{
}

bool SetObjectItemCommand::execute()
{
    return target->executeSetConfig(propName, newValue);
}

bool SetObjectItemCommand::unExecute()
{
    return target->executeSetConfig(propName, oldValue);
}

QString SetObjectItemCommand::getExecutionLog()
{
    return QString("%1,%2,Set,ConfigName:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(propName)
        .arg(oldValue.toString())
        .arg(newValue.toString());
}

QString SetObjectItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Set,ConfigName:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(propName)
        .arg(newValue.toString())
        .arg(oldValue.toString());
}

ConfigObject::ConfigObject(QObject *parent) : ConfigBase(ConfigElementInfo::ConfigObj, parent) {}

void ConfigObject::setOptionalProperty(QString configName, QVariantList options)
{
    m_configOptions[configName] = options;
}

void ConfigObject::setReadOnlyProperty(QString configName)
{
    m_readOnlyConfigNames.append(configName);
}

void ConfigObject::hide(QString configName)
{
    m_hiddenConfigNames.append(configName);
}

void ConfigObject::init()
{
    qDebug() << metaObject()->className() << " Start init";
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop(metaObject()->property(i));
        QString propName(QString(prop.name()));
        m_configProperties[propName] = prop;

        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            m_configTypes[propName] = propValue.value<ConfigBase *>()->configType();
        }
        else
        {
            m_configTypes[propName] = ConfigElementInfo::getType(prop.type());
        }
        if (propName != QString("objectName") && !m_hiddenConfigNames.contains(propName))
        {
            m_configNamesToShow.append(propName);
        }
    }

    foreach (auto readOnlyConfig, m_readOnlyConfigNames)
    {
        if (m_configOptions.contains(readOnlyConfig))
        {
            qCritical() << QString("Config can not be ReadOnly and Optional at the same time! Config name: %1")
                               .arg(readOnlyConfig);
        }
    }
    foreach (auto configName, m_configNamesToShow)
    {
        if (m_configTypes[configName] == ConfigElementInfo::Bool && m_configOptions.contains(configName))
        {
            qCritical() << QString("Bool config can not be optional! Config name: %1").arg(configName);
        }
    }

    isInit = true;
    qDebug() << metaObject()->className() << " End init";
}

void ConfigObject::connectNotifySignal(QString configName, QQuickItem *receiver, QString slotName)
{
    auto prop = m_configProperties[configName];
    if (!prop.hasNotifySignal())
    {
        qCritical() << "Did not have notify signal! Config name: " << configName;
        return;
    }
    //        int slotIndex =
    //        receiver->metaObject()->indexOfSlot(slotName.toStdString().c_str());
    int slotIndex = -1;
    for (int i = 0; i < receiver->metaObject()->methodCount(); i++)
    {
        auto method = receiver->metaObject()->method(i);
        if (method.methodType() == QMetaMethod::Slot && QString(method.name()) == slotName)
        {
            slotIndex = method.methodIndex();
            break;
        }
    }
    if (slotIndex == -1)
    {
        qCritical() << "Did not have slot! Slot name: " << slotName;
        return;
    }
    if (receiver->metaObject()->method(slotIndex).parameterCount() != 0)
    {
        qWarning() << "Slot method having 0 paremeter is recommoned! Slot name: " << slotName;
    }
    QMetaObject::connect(this, prop.notifySignalIndex(), receiver, slotIndex);
}

QStringList ConfigObject::getConfigNamesToShow() const
{
    return m_configNamesToShow;
}

bool ConfigObject::isReadOnly(QString configName) const
{
    return m_readOnlyConfigNames.contains(configName);
}

bool ConfigObject::isOptional(QString configName) const
{
    return m_configOptions.contains(configName);
}

ConfigElementInfo::Type ConfigObject::getConfigType(QString configName) const
{
    return m_configTypes[configName];
}

QVariantList ConfigObject::getOptions(QString configName) const
{
    return m_configOptions[configName];
}

QVariant ConfigObject::getConfig(QString configName) const
{
#ifdef ConfigUnitTest
    qDebug() << QString("Get config, name: %1 ").arg(configName)
             << "Value: " << QObject::property(configName.toStdString().c_str());
#endif

    return QObject::property(configName.toStdString().c_str());
}

bool ConfigObject::setConfig(QString configName, QVariant value)
{
    QVariant oldValue = QObject::property(configName.toStdString().c_str());
    if (oldValue == value)
    {
        return true;
    }
    return ConfigManager::getIns()->executeCommand(new SetObjectItemCommand(this, configName, oldValue, value));
}

bool ConfigObject::getIsInit() const
{
    return isInit;
}

bool ConfigObject::read(const QJsonValue &jsonValue)
{
    QJsonObject jsonObj = jsonValue.toObject();
    bool result = true;
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop = metaObject()->property(i);
        if (!jsonObj.contains(prop.name()))
        {
            qWarning() << QString("Read config value from json failed! Did not find key: %1").arg(prop.name());
            result = false;
            continue;
        }
        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            if (!propValue.value<ConfigBase *>()->read(jsonObj[prop.name()]))
            {
                result = false;
            }
        }
        else
        {
            if (!QObject::setProperty(prop.name(), jsonObj[prop.name()].toVariant()))
            {
                qWarning() << QString("Read config value from json failed! Config name: %1").arg(prop.name());
                result = false;
            }
        }
    }
    return result;
}

void ConfigObject::write(QJsonValue &jsonValue)
{
    QJsonObject jsonObj;
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop = metaObject()->property(i);
        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            QJsonValue subJsonValue;
            propValue.value<ConfigBase *>()->write(subJsonValue);
            jsonObj[prop.name()] = subJsonValue;
        }
        else
        {
            jsonObj[prop.name()] = propValue.toJsonValue();
        }
    }
    jsonValue = QJsonValue(jsonObj);
}

bool ConfigObject::connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect)
{
    if (!isInit)
    {
        qCritical() << "ConfigObject is not initialized!";
        return false;
    }

    foreach (auto prop, m_configProperties.values())
    {
        auto propValue = QObject::property(prop.name());
        if (propValue.canConvert<ConfigBase *>())
        {
            propValue.value<ConfigBase *>()->connectConfigChangedSignalToSlot(receiver, slotIndex, connect);
        }
        else
        {
            if (prop.hasNotifySignal())
            {
                if (connect)
                {
                    QMetaObject::connect(this, prop.notifySignalIndex(), receiver, slotIndex);
                }
                else
                {
                    QMetaObject::disconnect(this, prop.notifySignalIndex(), receiver, slotIndex);
                }
            }
        }
    }
    return true;
}

bool ConfigObject::executeSetConfig(QString configName, QVariant value)
{
#ifdef ConfigUnitTest
    qDebug() << QString("Set config, name: %1, value: ").arg(configName) << value;
#endif

    bool result = QObject::setProperty(configName.toStdString().c_str(), value);
    if (!result)
    {
        qCritical() << QString("Set config failed! Config name: %1").arg(configName);
    }
    return result;
}
