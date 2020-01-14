#ifndef UTILITY_H
#define UTILITY_H

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QTextCodec>
#include <Windows.h>
#include <iostream>

class LogLevel
{
public:
    enum Level
    {
        Debug = 0,
        Info,
        Warning,
        Critical,
        Fatal
    };

private:
    LogLevel()
    {
        levelToString[Debug] = "Debug";
        levelToString[Info] = "Info";
        levelToString[Warning] = "Warning";
        levelToString[Critical] = "Critical";
        levelToString[Fatal] = "Fatal";
    }

public:
    static const LogLevel &getIns()
    {
        static const LogLevel instance;
        return instance;
    }

    QString toString(int level) const
    {
        return levelToString[level];
    }

private:
    QMap<int, QString> levelToString;
};

class ConsoleOutputer
{
private:
    enum ConsoleColor
    {
        White,
        Yellow,
        Red
    };

public:
    void outputLogToConsole(int logLevel, QString &msg)
    {
        if (logLevel < LogLevel::Warning)
        {
            if (lastConsoleColor != White)
            {
                setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
                lastConsoleColor = White;
            }
            std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
        }
        else if (logLevel == LogLevel::Warning)
        {
            if (lastConsoleColor != Yellow)
            {
                setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED);
                lastConsoleColor = Yellow;
            }
            std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
        }
        else
        {
            if (lastConsoleColor != Red)
            {
                setConsoleColor(FOREGROUND_RED);
                lastConsoleColor = Red;
            }
            std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
        }
    }

private:
    inline void setConsoleColor(unsigned short forecolor = 4, unsigned short backgroudcolor = 0)
    {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);                //获取缓冲区句柄
        SetConsoleTextAttribute(hCon, forecolor | backgroudcolor);    //设置文本及背景色
    }

private:
    int lastConsoleColor = White;
};

#endif    // UTILITY_H
