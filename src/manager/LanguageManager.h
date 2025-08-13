#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QApplication>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTranslator>

class LanguageManager : public QObject
{
    Q_OBJECT

  public:
    static LanguageManager& instance();

    void loadLanguage(const QString& language);
    QString getCurrentLanguage() const;
    QStringList getAvailableLanguages() const;

  signals:
    void languageChanged(const QString& language);

  private:
    LanguageManager();

    QTranslator* translator;
    QString currentLanguage;
    QMap<QString, QString> languageFiles;

    void setupLanguageFiles();
};

#endif // LANGUAGEMANAGER_H
