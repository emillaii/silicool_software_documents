#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include "utilities_global.h"
#include <QDateTime>
#include <QtCore>

UTILITIESSHARED_EXPORT QString combineString(const QString &seperator, const QList<QString> &stringList);

template <typename T>
QString combineNumber(const QString &seperator, const QList<T> &numberList)
{
    QString returnString;
    for (int i = 0; i < numberList.count(); i++)
    {
        returnString += QString::number(numberList[i]);
        if (i != numberList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

inline QString getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

#define CHECK_POINTER(pointer)                                                                                         \
    if (pointer == nullptr)                                                                                            \
    {                                                                                                                  \
        qFatal("%s is null!", #pointer);                                                                               \
    }

#define SILICOOL_DELETE(pointer)                                                                                       \
    if (pointer != nullptr)                                                                                            \
    {                                                                                                                  \
        delete pointer;                                                                                                \
        pointer = nullptr;                                                                                             \
    }

#endif
