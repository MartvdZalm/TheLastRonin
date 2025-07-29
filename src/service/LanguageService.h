#ifndef LANGUAGESERVICE_H
#define LANGUAGESERVICE_H

#include <QApplication>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTranslator>

class LanguageService : public QObject
{
    Q_OBJECT

  public:
    static LanguageService& instance();

    void loadLanguage(const QString& language);
    QString getCurrentLanguage() const;
    QStringList getAvailableLanguages() const;

  signals:
    void languageChanged(const QString& language);

  private:
    LanguageService();

    QTranslator* translator;
    QString currentLanguage;
    QMap<QString, QString> languageFiles;

    void setupLanguageFiles();
};

#endif // LANGUAGESERVICE_H
