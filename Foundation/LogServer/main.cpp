#include "LogToFileHandler.h"
#include <QCoreApplication>
#include <QSharedMemory>
#include <WinUser.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto windowHandle = GetConsoleWindow();
    SetWindowTextA(windowHandle, "LogServer");
    RemoveMenu(GetSystemMenu(windowHandle, false), SC_CLOSE, 0x0);

    QSharedMemory shared("LogServer");
    if (shared.attach())
    {
        qDebug() << "LogServer is running!";
        return 0;
    }
    shared.create(1);

    LogToFileHandler logServer(true);
    logServer.init();

    return a.exec();
}
