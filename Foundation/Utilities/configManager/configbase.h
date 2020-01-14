#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include "../utilities_global.h"
#include <QJsonValue>
#include <QObject>
#include <QVariant>

#define ConfigUnitTest 1

class UTILITIESSHARED_EXPORT ConfigElementInfo : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        ConfigArr,
        ConfigObj,
        ConfigObjArr,

        Bool,      // UI will create a Checkbox to visualize the corresponding config
        Int,       // UI will create a TextField with a IntValidator to visualize the corresponding config
        Double,    // UI will create a TextField with a DoubleValidator to visualize the corresponding config
        Other
    };
    Q_ENUM(Type)

    enum Property
    {
        Optional,    // UI will create a ComboBox to visualize the corresponding config
        ReadOnly     // UI item`s enable will be set to false
    };
    Q_ENUM(Property)

    static Type getType(QVariant::Type qvType);
};

class UTILITIESSHARED_EXPORT ConfigFile;

class UTILITIESSHARED_EXPORT ConfigBase : public QObject
{
    Q_OBJECT

public:
    ConfigBase(ConfigElementInfo::Type type, QObject *parent = nullptr);

    Q_INVOKABLE ConfigElementInfo::Type configType() const;

    void connectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect);
    virtual bool read(const QJsonValue &jsonValue) = 0;
    virtual void write(QJsonValue &jsonValue) = 0;

protected:
    virtual bool connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect) = 0;

private:
    QString createReceiverSlotId(QObject *receiver, int slotIndex);

private:
    friend class ConfigFile;

    ConfigElementInfo::Type m_configType;
    QMap<QString, bool> configChangedSignalConnectionStates;
};

#endif    // CONFIGBASE_H
