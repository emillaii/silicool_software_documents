#include "configmanagerunittest.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qDebug("Hi");

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();

    QObjectFactory::registerClass<TestClassA>();
    QObjectFactory::registerClass<TestClassB>();
    QObjectFactory::registerClass<TestClassE>();

    engine.rootContext()->setContextProperty("configManager", ConfigManager::getIns());
    TestClassB testClassB;
    testClassB.setObjectName("testClassB");
    ConfigFile testClassBConfigFile("testClassB", &testClassB, "testClassB.json");
    testClassBConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassB", &testClassB);

    TestClassC testClassC;
    testClassC.setObjectName("testClassC");
    ConfigFile testClassCConfigFile("testClassC", &testClassC, "testClassC.json");
    testClassCConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassC", &testClassC);

    TestClassD testClassD;
    testClassD.setObjectName("testClassD");
    ConfigFile testClassDConfigFile("testClassD", &testClassD, "testClassD.json");
    testClassDConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassD", &testClassD);

    ConfigObjectArray testClassEArray("TestClassE");
    testClassEArray.setObjectName("testClassEArray");
    ConfigFile testClassEArrayConfigFile("testClassEArray", &testClassEArray, "testClassEArray.json");
    testClassEArrayConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassEArray", &testClassEArray);

    ConfigArray intArray(ConfigElementInfo::Int);
    intArray.setObjectName("intArray");
    ConfigFile intArrayConfigFile("intArray", &intArray, "intArray.json");
    intArrayConfigFile.populate();
    engine.rootContext()->setContextProperty("intArray", &intArray);
    QVariantList intOptions;
    for (int i = 10; i < 20; i++)
    {
        intOptions.append(i);
    }
    intArray.setOptionalProperty(intOptions);

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
