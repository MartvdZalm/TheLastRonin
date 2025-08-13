#include "LanguageManager.h"

#include <QDebug>
#include <QDir>

LanguageManager::LanguageManager() : translator(nullptr), currentLanguage("English")
{
    setupLanguageFiles();
}

LanguageManager& LanguageManager::instance()
{
    static LanguageManager instance;
    return instance;
}

void LanguageManager::setupLanguageFiles()
{
    languageFiles["English"] = "";
    languageFiles["Dutch"] = ":/translations/app_nl.qm";
    languageFiles["Japanese"] = ":/translations/app_jp.qm";
}

void LanguageManager::loadLanguage(const QString& language)
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

QString LanguageManager::getCurrentLanguage() const
{
    return currentLanguage;
}

QStringList LanguageManager::getAvailableLanguages() const
{
    return languageFiles.keys();
}
