// SettingsDAO.h
#ifndef SETTINGSDAO_H
#define SETTINGSDAO_H

#include "../database/DatabaseManager.h"
#include <QString>

class SettingsDAO
{
  public:
    SettingsDAO();

    QString getSetting(const QString& key);
    bool setSetting(const QString& key, const QString& value);

  private:
    DatabaseManager& db;
};

#endif // SETTINGSDAO_H
