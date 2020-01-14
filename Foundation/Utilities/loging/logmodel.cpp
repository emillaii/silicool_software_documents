#include "logmodel.h"

LogModel::LogModel()
{
    logRoleNames[LogModelColorRole] = "logColor";
    logRoleNames[LogModelLogRole] = "logStr";
}

void LogModel::onLogBufferChanged(QList<LogItem *> logBuffer)
{
    if (logs.count() > MAX_LINES)
    {
        removeFrontRows(N_REMOVE_LINES);
    }

    beginInsertRows(QModelIndex(), logs.count(), logs.count() + logBuffer.count() - 1);
    foreach (auto logItem, logBuffer)
    {
        logs.append(logItem);
    }
    endInsertRows();
}

void LogModel::onClearLog()
{
    if (logs.count() > 0)
    {
        removeFrontRows(logs.count());
    }
}

void LogModel::removeFrontRows(int rowCount)
{
    beginRemoveRows(QModelIndex(), 0, rowCount - 1);
    for (int i = 0; i < rowCount; i++)
    {
        LogItem *logItem = logs.last();
        logs.removeLast();
        delete logItem;
    }
    endRemoveRows();
}
