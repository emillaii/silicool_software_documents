#include "lensloader.h"
#include "lenspickarm.h"
#include "trayloader.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QObjectFactory::registerClass<LogCategoryConfig>();
    Logger::getIns()->init(true);

    QQmlApplicationEngine engine;

    LensLoader lensLoader;
    LensPickArm lensPickArm;
    TrayLoader trayLoader;

    engine.rootContext()->setContextProperty("lensLoader", &lensLoader);
    engine.rootContext()->setContextProperty("lensPickArm", &lensPickArm);
    engine.rootContext()->setContextProperty("trayLoader", &trayLoader);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
