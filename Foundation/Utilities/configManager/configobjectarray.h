#ifndef CONFIGOBJECTARRAY_H
#define CONFIGOBJECTARRAY_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "configobject.h"
#include "qobjectfactory.h"
#include <QAbstractTableModel>

class UTILITIESSHARED_EXPORT ConfigObjectArray;

class AddConfigObjectComand : public CommandInterface
{
public:
    AddConfigObjectComand(ConfigObjectArray *target, int index) : target(target), index(index) {}

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray *target;
    int index;
};

class RemoveConfigObjectCommand : public CommandInterface
{
public:
    RemoveConfigObjectCommand(ConfigObjectArray *target, int index, ConfigObject *oldConfigObj)
        : target(target), index(index), oldConfigObj(oldConfigObj)
    {
    }

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray *target;
    int index;
    ConfigObject *oldConfigObj;
};

class UTILITIESSHARED_EXPORT ConfigObjectArray : public ConfigBase
{
    Q_OBJECT

public:
    ///
    /// \brief ConfigObjectArray
    /// \param configObjClassName This class must inherit ConfibObject, and the parameter of it`s constructor must be
    /// (QObject* parent) \param parent
    ///
    ConfigObjectArray(QString configObjClassName, QObject *parent = nullptr);

    ConfigObject *const &at(int index) const;
    ConfigObject *const &operator[](int index) const;
    ConfigObject *&operator[](int index);
    int count() const;

    Q_INVOKABLE bool add(int index);

    Q_INVOKABLE bool remove(int index);

    Q_INVOKABLE int configCount() const;

    Q_INVOKABLE QStringList getTitles() const;

    Q_INVOKABLE QObject *getConfig(int index) const;

    // ConfigBase interface
public:
    virtual bool read(const QJsonValue &jsonValue) override;
    virtual void write(QJsonValue &jsonValue) override;
    bool connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect) override;

signals:
    void configInserted(int index, ConfigObject *value);
    void configRemoved(int index);

private:
    bool executeAddConfigObject(int index, ConfigObject *instance = nullptr);
    bool executeRemoveConfigObject(int index);

private:
    friend class AddConfigObjectComand;
    friend class RemoveConfigObjectCommand;

    QString configObjClassName;
    QList<ConfigObject *> configObjects;
    ConfigObject *templateObject;

    QObject *configChangedSignalReceiver = nullptr;
    int configChangedReceiverSlotIndex;
};

#endif    // CONFIGOBJECTARRAY_H
