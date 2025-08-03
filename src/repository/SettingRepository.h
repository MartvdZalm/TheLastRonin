#ifndef SETTINGREPOSITORY_H
#define SETTINGREPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../model/Setting.h"
#include "Repository.h"
#include <QObject>

class SettingRepository : public QObject
{
    Q_OBJECT

  public:
    explicit SettingRepository(QObject* parent = nullptr);

    bool save(Setting* setting);
    std::unique_ptr<Setting> find(int id);
    std::vector<std::unique_ptr<Setting>> findAll();
    std::unique_ptr<Setting> findByKey(const QString& key);
    bool updateValueByKey(const QString& key, const QString& newValue);

  private:
    std::shared_ptr<Repository<Setting>> baseRepository;
};

#endif // SETTINGREPOSITORY_H
