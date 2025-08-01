#ifndef SETTINGSREPOSITORY_H
#define SETTINGSREPOSITORY_H

#include "../database/DatabaseManager.h"
#include <QString>

class SettingsRepository
{
  public:
    SettingsRepository();

    QString getSetting(const QString& key);
    bool setSetting(const QString& key, const QString& value);

  private:
    DatabaseManager& db;
};

#endif // SETTINGSREPOSITORY_H
