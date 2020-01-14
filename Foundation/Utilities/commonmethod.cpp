#include "commonmethod.h"

QString combineString(const QString &seperator, const QList<QString> &stringList)
{
    QString returnString;
    for (int i = 0; i < stringList.count(); i++)
    {
        returnString += stringList[i];
        if (i != stringList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}
