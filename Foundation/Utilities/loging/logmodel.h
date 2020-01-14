#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "../utilities_global.h"
#include "LogBuffer.h"
#include <QAbstractListModel>

enum LogModelRole
{
    LogModelColorRole = Qt::UserRole + 1,
    LogModelLogRole
};

class UTILITIESSHARED_EXPORT LogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LogModel();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return logs.count();
    }

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.row() >= 0 && index.row() < logs.count())
        {
            if (role == LogModelColorRole)
            {
                return QVariant(logs[index.row()]->color);
            }
            if (role == LogModelLogRole)
            {
                return QVariant(logs[index.row()]->log);
            }
        }
        return QVariant();
    }

    virtual QHash<int, QByteArray> roleNames() const override
    {
        return logRoleNames;
    }

public slots:
    void onLogBufferChanged(QList<LogItem *> logBuffer);
    void onClearLog();

private:
    void removeFrontRows(int rowCount);

private:
    const int MAX_LINES = 1500;
    const int N_REMOVE_LINES = 500;
    QList<LogItem *> logs;
    QHash<int, QByteArray> logRoleNames;
};

#endif    // LOGMODEL_H
