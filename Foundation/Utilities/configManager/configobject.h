#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "configbase.h"
#include "configmanager.h"
#include <QDebug>
#include <QJsonObject>
#include <QMetaProperty>
#include <QQuickItem>
#include <QVariant>

class UTILITIESSHARED_EXPORT ConfigObject;

class SetObjectItemCommand : public CommandInterface
{
public:
    SetObjectItemCommand(ConfigObject *target, QString &propName, QVariant &oldValue, QVariant &newValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObject *target;
    QString propName;
    QVariant oldValue;
    QVariant newValue;
};

class UTILITIESSHARED_EXPORT ConfigObject : public ConfigBase
{
    Q_OBJECT

public:
    ConfigObject(QObject *parent = nullptr);

    ///
    /// \brief setOptionalProperty This function should be called before calling
    /// init function \param configName   QT Property name \param options  The
    /// options will be the model of the corresponding ComboBox
    ///
    void setOptionalProperty(QString configName, QVariantList options);

    ///
    /// \brief setReadOnlyProperty  This function should be called before
    /// calling init function \param configName
    ///
    void setReadOnlyProperty(QString configName);

    ///
    /// \brief The hidden item will not be shown on UI.
    /// This function should be called before calling init function
    /// \param configName  QT Property name
    ///
    void hide(QString configName);

    ///
    /// \brief init Init must be called, otherwise the UI will not work.
    /// Init must be called after calling setOptionalProperty,
    /// setReadOnlyProperty and hide Calling this function in subclass`s
    /// constructor is recommended. If the subclass of ConfigObject has property
    /// which is pointer of ConfigObject or ConfigArray, make sure these
    /// pointers have been initialized! Otherwise the software may crashed!
    ///
    /// Warning: When you define an static instance of a class inheriting
    /// ConfigObject, you can not call this method in the constructor of that
    /// class! Otherwise software will crash. Because QT`s metaobject-system may
    /// not be initialized when constructing the static instance, i guess.
    ///
    void init();

    Q_INVOKABLE void connectNotifySignal(QString configName, QQuickItem *receiver, QString slotName);

    Q_INVOKABLE QStringList getConfigNamesToShow() const;

    Q_INVOKABLE bool isReadOnly(QString configName) const;

    Q_INVOKABLE bool isOptional(QString configName) const;

    Q_INVOKABLE ConfigElementInfo::Type getConfigType(QString configName) const;

    Q_INVOKABLE QVariantList getOptions(QString configName) const;

    Q_INVOKABLE QVariant getConfig(QString configName) const;

    Q_INVOKABLE bool setConfig(QString configName, QVariant value);

    bool getIsInit() const;

    bool read(const QJsonValue &jsonValue) override;
    void write(QJsonValue &jsonValue) override;
    bool connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect) override;

private:
    bool executeSetConfig(QString configName, QVariant value);

private:
    friend class ConfigFile;
    friend class SetObjectItemCommand;

    bool isInit = false;
    QStringList m_configNamesToShow;
    QStringList m_hiddenConfigNames;
    QStringList m_readOnlyConfigNames;
    QMap<QString, ConfigElementInfo::Type> m_configTypes;
    QMap<QString, QVariantList> m_configOptions;
    QMap<QString, QMetaProperty> m_configProperties;
};

#endif    // CONFIGOBJECT_H
