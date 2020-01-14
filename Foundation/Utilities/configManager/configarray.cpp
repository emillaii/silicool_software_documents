#include "configarray.h"

SetArrayItemCommand::SetArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue, QVariant &newValue)
    : target(target), index(index), oldValue(oldValue), newValue(newValue)
{
}

bool SetArrayItemCommand::execute()
{
    return target->executeSetConfig(index, newValue);
}

bool SetArrayItemCommand::unExecute()
{
    return target->executeSetConfig(index, oldValue);
}

QString SetArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Set,Index:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(oldValue.toString())
        .arg(newValue.toString());
}

QString SetArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Set,Index:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(newValue.toString())
        .arg(oldValue.toString());
}

AddArrayItemCommand::AddArrayItemCommand(ConfigArray *target, int index) : target(target), index(index) {}

bool AddArrayItemCommand::execute()
{
    return target->executeAddItem(index);
}

bool AddArrayItemCommand::unExecute()
{
    return target->executeRemoveItem(index);
}

QString AddArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Add,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

QString AddArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

RemoveArrayItemCommand::RemoveArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue)
    : target(target), index(index), oldValue(oldValue)
{
}

bool RemoveArrayItemCommand::execute()
{
    return target->executeRemoveItem(index);
}

bool RemoveArrayItemCommand::unExecute()
{
    return target->executeAddItem(index) && target->executeSetConfig(index, oldValue);
}

QString RemoveArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3,Value:%4")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(oldValue.toString());
}

QString RemoveArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Insert,Index:%3,Value:%4")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(oldValue.toString());
}

ConfigArray::ConfigArray(ConfigElementInfo::Type elementType, QObject *parent)
    : ConfigBase(ConfigElementInfo::ConfigArr, parent), elementType(elementType)
{
    if (elementType == ConfigElementInfo::ConfigArr)
    {
        qFatal("The element of ConfigArray can not be another ConfigArray!");
    }
    if (elementType == ConfigElementInfo::ConfigObj)
    {
        qFatal("The element of ConfigArray can not be ConfigObj! You can use ConfigObjectArray instead.");
    }
}

void ConfigArray::setOptionalProperty(QVariantList &options)
{
    if (elementType == ConfigElementInfo::Bool)
    {
        qCritical() << "Array element of bool type can not be set to optional!";
        return;
    }
    if (m_isReadOnly)
    {
        qCritical() << "Array element can not be optional and read only at the same time!";
        return;
    }
    if (options.count() == 0)
    {
        qCritical() << "Options is empty!";
        return;
    }
    auto optionType = ConfigElementInfo::getType(options[0].type());
    if (optionType != elementType)
    {
        qCritical() << QString("The type of option is not the same as element type! Option type: %1, element type: %2")
                           .arg(optionType)
                           .arg(elementType);
        return;
    }
    m_isOptional = true;
    m_options = options;
}

bool ConfigArray::setConfig(int index, QVariant value)
{
    if (index < 0 || index >= members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(members.count());
        return false;
    }

    if (elementType == ConfigElementInfo::Int)
    {
        if (value.canConvert<int>())
        {
            value = value.toInt();
        }
        else
        {
            qCritical() << QString("Can not convert to type int. value: ") << value;
            return false;
        }
    }

    if (elementType == ConfigElementInfo::Double)
    {
        if (value.canConvert<double>())
        {
            value = value.toDouble();
        }
        else
        {
            qCritical() << QString("Can not convert to type double. value: ") << value;
            return false;
        }
    }

    if (elementType == ConfigElementInfo::Bool)
    {
        if (value.canConvert<bool>())
        {
            value = value.toBool();
        }
        else
        {
            qCritical() << QString("Can not convert to type bool. value: ") << value;
            return false;
        }
    }

    QVariant oldValue = members[index];
    if (oldValue == value)
    {
        return true;
    }
    return ConfigManager::getIns()->executeCommand(new SetArrayItemCommand(this, index, oldValue, value));
}

bool ConfigArray::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();
    members.clear();
    bool result = true;
    for (int i = 0; i < jsonArr.count(); i++)
    {
        if (!executeAddItem(i))
        {
            return false;
        }
        QVariant jsonValue = jsonArr[i].toVariant();
        if (!setSimpleTypeConfig(i, jsonValue))
        {
            result = false;
        }
    }
    return result;
}

void ConfigArray::write(QJsonValue &jsonValue)
{
    QJsonArray jsonArr;

    for (int i = 0; i < members.count(); i++)
    {
        jsonArr.append(members[i].toJsonValue());
    }
    jsonValue = QJsonValue(jsonArr);
}

bool ConfigArray::setSimpleTypeConfig(int index, QVariant &value)
{
    if (index < 0 || index >= members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(members.count());
        return false;
    }

    auto ValueType = ConfigElementInfo::getType(value.type());
    if (ValueType == ConfigElementInfo::Double && elementType == ConfigElementInfo::Int)
    {
        value = value.toInt();
    }
    else if (ValueType == ConfigElementInfo::Int && elementType == ConfigElementInfo::Double)
    {
        value = value.toDouble();
    }
    else
    {
        if (ValueType != elementType)
        {
            qCritical() << QString("Set config failed! Index: %1, Value type: %2, target type: %3")
                               .arg(index)
                               .arg(ValueType)
                               .arg(elementType);
            return false;
        }
    }
    members.replace(index, value);
    return true;
}

bool ConfigArray::connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect)
{
    for (int i = 0; i < metaObject()->methodCount(); i++)
    {
        auto method = metaObject()->method(i);
        if (method.methodType() == QMetaMethod::Signal)
        {
            if (QString(method.name()) == QString("configInserted")
                || QString(method.name()) == QString("configRemoved")
                || QString(method.name()) == QString("configChanged"))
            {
                if (connect)
                {
                    QMetaObject::connect(this, method.methodIndex(), receiver, slotIndex);
                }
                else
                {
                    QMetaObject::disconnect(this, method.methodIndex(), receiver, slotIndex);
                }
            }
        }
    }
    return true;
}

bool ConfigArray::executeSetConfig(int index, QVariant value)
{
#ifdef ConfigUnitTest
    qDebug() << QString("Set config, index: %1, value: ").arg(index) << value;
#endif

    if (setSimpleTypeConfig(index, value))
    {
        emit configChanged(index, members[index]);
        return true;
    }
    else
    {
        return false;
    }
}

bool ConfigArray::executeAddItem(int index)
{
    if (index < 0 || index > members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(members.count());
        return false;
    }

#ifdef ConfigUnitTest
    qDebug() << QString("Add config, index: %1 ").arg(index);
#endif

    QVariant qv;
    if (elementType == ConfigElementInfo::Int)
    {
        qv = QVariant::fromValue(0);
    }
    else if (elementType == ConfigElementInfo::Double)
    {
        qv = QVariant::fromValue(0.0);
    }
    else if (elementType == ConfigElementInfo::Bool)
    {
        qv = QVariant::fromValue(false);
    }
    else
    {
        qv = QVariant::fromValue(QString());
    }
    members.insert(index, qv);
    emit configInserted(index, qv);
    return true;
}

bool ConfigArray::executeRemoveItem(int index)
{
    if (index < 0 || index >= members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(members.count());
        return false;
    }

#ifdef ConfigUnitTest
    qDebug() << QString("Remove config, index: %1 ").arg(index);
#endif

    members.removeAt(index);
    emit configRemoved(index);
    return true;
}
