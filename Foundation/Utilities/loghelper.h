#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QDebug>
#include <ostream>
#include <string>
#include <strstream>

inline void output2Stream(std::strstream &stream, QString &t)
{
    stream << t.toUtf8().data();
}

inline void output2Stream(std::strstream &stream, std::string &t)
{
    stream << t.c_str();
}

template <typename T>
void output2Stream(std::strstream &stream, T t)
{
    stream << t;
}

template <typename T>
void combineArgNameValue(std::strstream &stream, QStringList &argNameList, int index, T first)
{
    stream << argNameList[index].toUtf8().data() << ": ";
    output2Stream(stream, first);
}

template <typename T, typename... Types>
void combineArgNameValue(std::strstream &stream, QStringList &argNameList, int index, T first, Types... rest)
{
    stream << argNameList[index].toUtf8().data() << ": ";
    output2Stream(stream, first);
    stream << ", ";
    combineArgNameValue(stream, argNameList, index + 1, rest...);
}

template <typename... Args>
QString formatArgs(const char *argNames, Args... args)
{
    auto argNameList = QString(argNames).split(',');
    char buffer[1024] = { 0 };
    std::strstream stream(buffer, sizeof(buffer));
    combineArgNameValue(stream, argNameList, 0, args...);
    return QString::fromUtf8(buffer);
}

#define FUNC "Enter func: %s.", __FUNCTION__
#define FUNC_ARG(...) "Enter func: %s, Args: %s.", __FUNCTION__, formatArgs(#__VA_ARGS__, __VA_ARGS__).toUtf8().data()

#endif    // LOGHELPER_H
