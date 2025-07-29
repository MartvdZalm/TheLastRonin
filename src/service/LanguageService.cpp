#include "LanguageService.h"

#include <QDebug>
#include <QDir>

LanguageService::LanguageService() : translator(nullptr), currentLanguage("English")
{
    setupLanguageFiles();
}

LanguageService& LanguageService::instance()
{
    static LanguageService instance;
    return instance;
}

void LanguageService::setupLanguageFiles()
{
    languageFiles["English"] = "";
    languageFiles["Dutch"] = ":/translations/app_nl.qm";
    languageFiles["Japanese"] = ":/translations/app_jp.qm";
}

void LanguageService::loadLanguage(const QString& language)
{
    if (currentLanguage == language)
        return;

    if (translator)
    {
        QApplication::removeTranslator(translator);
        delete translator;
        translator = nullptr;
    }

    if (language != "English" && languageFiles.contains(language))
    {
        translator = new QTranslator;
        QString translationFile = languageFiles[language];

        qDebug() << "Attempting to load translation:" << translationFile;

        if (translator->load(translationFile))
        {
            QApplication::installTranslator(translator);
            qDebug() << "Successfully loaded translation:" << translationFile;
        }
    }

    currentLanguage = language;
    emit languageChanged(language);
}

QString LanguageService::getCurrentLanguage() const
{
    return currentLanguage;
}

QStringList LanguageService::getAvailableLanguages() const
{
    return languageFiles.keys();
}
