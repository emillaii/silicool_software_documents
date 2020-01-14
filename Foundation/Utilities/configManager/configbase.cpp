#include "configbase.h"

ConfigElementInfo::Type ConfigElementInfo::getType(QVariant::Type qvType)
{
    if (qvType == QVariant::Int || qvType == QVariant::UInt || qvType == QVariant::LongLong
        || qvType == QVariant::ULongLong)
    {
        return Int;
    }
    else if (qvType == QVariant::Double)
    {
        return Double;
    }
    else if (qvType == QVariant::Bool)
    {
        return Bool;
    }
    else
    {
        return Other;
    }
}

ConfigBase::ConfigBase(ConfigElementInfo::Type type, QObject *parent) : QObject(parent), m_configType(type) {}

ConfigElementInfo::Type ConfigBase::configType() const
{
    return m_configType;
}

void ConfigBase::connectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect)
{
    // Qt did not check if signal was connected to slot.
    // If we connect the signal and slot more than once,
    // one signal emiting would cause multi slot called.
    QString receiverSlotID = createReceiverSlotId(receiver, slotIndex);
    if (configChangedSignalConnectionStates.contains(receiverSlotID)
        && configChangedSignalConnectionStates[receiverSlotID] == connect)
    {
        return;
    }
    if (connectConfigChangedSignalToSlotImpl(receiver, slotIndex, connect))
    {
        configChangedSignalConnectionStates[receiverSlotID] = connect;
    }
}

QString ConfigBase::createReceiverSlotId(QObject *receiver, int slotIndex)
{
    // Todo  would crash when running on 32-bit os.
    qint64 *receiverAddress = reinterpret_cast<qint64 *>(&receiver);
    return QString::number(*receiverAddress) + QString::number(slotIndex);
}
