#include "SettingRepository.h"

#include "../database/ORM.h"
#include "../database/QueryBuilder.h"

SettingRepository::SettingRepository(QObject* parent) : QObject(parent)
{
    baseRepository = ORM::instance().getRepository<Setting>();
}

bool SettingRepository::save(Setting* playlist)
{
    return baseRepository->save(playlist);
}

std::unique_ptr<Setting> SettingRepository::find(int id)
{
    return baseRepository->find(id);
}

QList<std::unique_ptr<Setting>> SettingRepository::findAll()
{
    return baseRepository->findAll();
}

std::unique_ptr<Setting> SettingRepository::findByKey(const QString& key)
{
    QSqlRecord record = QueryBuilder::table("settings").where("key", "=", key).limit(1).first();

    if (record.isEmpty())
    {
        return nullptr;
    }

    auto setting = std::make_unique<Setting>();
    setting->deserialize(record);
    return setting;
}

bool SettingRepository::updateValueByKey(const QString& key, const QString& newValue)
{
    std::unique_ptr<Setting> settingModel = findByKey(key);
    if (!settingModel)
    {
        return false;
    }

    settingModel->setValue(newValue);
    return save(settingModel.get());
}