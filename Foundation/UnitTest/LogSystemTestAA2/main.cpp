#include "sensorloader.h"
#include "sensorpickarm.h"
#include "sensortrayloader.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QObjectFactory::registerClass<LogCategoryConfig>();

    Logger::getIns()->init(true);

    QQmlApplicationEngine engine;

    SensorLoader sensorLoader;
    SensorPickArm sensorPickArm;
    SensorTrayLoader sensorTrayLoader;

    engine.rootContext()->setContextProperty("sensorLoader", &sensorLoader);
    engine.rootContext()->setContextProperty("sensorPickArm", &sensorPickArm);
    engine.rootContext()->setContextProperty("sensorTrayLoader", &sensorTrayLoader);

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
