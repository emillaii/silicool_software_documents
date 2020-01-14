#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include "../utilities_global.h"
#include "languageconfig.h"
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>

class UTILITIESSHARED_EXPORT LanguageManager : public QObject
{
    Q_OBJECT

public:
    LanguageManager(LanguageConfig *languageConfig, QGuiApplication &app, QQmlApplicationEngine &engine)
        : languageConfig(languageConfig), app(app), engine(engine)
    {
        connect(languageConfig, &LanguageConfig::languageChanged, this, &LanguageManager::onLanguageChanged);
    }

private slots:
    void onLanguageChanged(int language)
    {
        QTranslator translator;
        switch (language)
        {
            case LanguageConfig::Chinese:
                translator.load(":/languagePackage/chinese.qm");
                break;
            case LanguageConfig::English:
                translator.load(":/languagePackage/english.qm");
                break;
            default:
                qCritical("Unknown language: %d", language);
                return;
        }
        app.installTranslator(&translator);
        engine.retranslate();
    }

private:
    LanguageConfig *languageConfig;
    QGuiApplication &app;
    QQmlApplicationEngine &engine;
};

#endif    // LANGUAGEMANAGER_H
