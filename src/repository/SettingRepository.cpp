#include "SettingRepository.h"

#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>
#include <iostream>
#include <QSqlError>

SettingRepository::SettingRepository(QSqlDatabase& db) : database(db) {}

std::optional<Setting> SettingRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM settings WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        qCritical() << "Database error when finding setting ID" << id << ":" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next())
    {
        auto setting = mapFromRecord(query);
        qDebug() << "Successfully found setting ID:" << id;
        return setting;
    }

    qDebug() << "Setting not found with ID:" << id;
    return std::nullopt;
}

std::optional<Setting> SettingRepository::findByKey(const QString& key)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM settings WHERE key = :key");
    query.bindValue(":key", key);

    if (!query.exec())
    {
        qCritical() << "Database error when finding setting with key" << key << ":" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next()) {
        auto setting = mapFromRecord(query);
        qDebug() << "Successfully found setting ID:" << setting.getId();
        return setting;
    }

    qDebug() << "Setting not found with key:" << key;
    return std::nullopt;
}

QString SettingRepository::getValue(const QString& key, const QString& defaultValue)
{
    auto setting = findByKey(key);
    return setting ? setting->getValue() : defaultValue;
}

std::optional<Setting> SettingRepository::setValue(const QString& key, const QString& value)
{
    auto existing = findByKey(key);

    if (existing)
    {
        Setting updated = existing.value();
        updated.setValue(value);
        updated.setUpdatedAt(QDateTime::currentDateTime());
        return update(updated);
    }

    Setting newSetting;
    newSetting.setKey(key);
    newSetting.setValue(value);
    return insert(newSetting);
}

std::optional<Setting> SettingRepository::save(const Setting& setting)
{
    return setting.getId() > 0 ? update(setting) : insert(setting);
}

QList<Setting> SettingRepository::findAll()
{
    QList<Setting> results;
    QSqlQuery query("SELECT * FROM settings", database);

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    return results;
}

QList<QString> SettingRepository::getAllKeys()
{
    QList<QString> keys;
    QSqlQuery query("SELECT key FROM settings", database);

    while (query.next())
    {
        keys.append(query.value("key").toString());
    }

    return keys;
}

bool SettingRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM settings WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec())
    {
        qCritical() << "Failed to delete setting ID" << id << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully deleted setting ID:" << id;
    return query.exec();
}

bool SettingRepository::deleteByKey(const QString& key)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM settings WHERE key = ?");
    query.addBindValue(key);

    if (!query.exec())
    {
        qCritical() << "Failed to delete setting by key " << key << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully deleted setting key:" << key;
    return query.exec();
}

Setting SettingRepository::mapFromRecord(const QSqlQuery& query)
{
    Setting setting;
    setting.setId(query.value("id").toInt());
    setting.setKey(query.value("key").toString());
    setting.setValue(query.value("value").toString());
    setting.setCreatedAt(query.value("created_at").toDateTime());
    setting.setUpdatedAt(query.value("updated_at").toDateTime());
    return setting;
}

std::optional<Setting> SettingRepository::insert(const Setting& setting)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO settings (key, value) VALUES (:key, :value)");

    query.bindValue(":key", setting.getKey());
    query.bindValue(":value", setting.getValue());

    if (!query.exec())
    {
        qCritical() << "Failed to insert setting '" << setting.getKey() << "':" << query.lastError().text();
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    qDebug() << "Successfully inserted new setting ID:" << id;
    return findById(id);
}

std::optional<Setting> SettingRepository::update(const Setting& setting)
{
    qDebug() << setting.getKey();
    qDebug() << setting.getValue();
    qDebug() << setting.getUpdatedAt();
    qDebug() << setting.getId();

    QSqlQuery query(database);
    query.prepare("UPDATE settings SET key = :key, value = :value, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":key", setting.getKey());
    query.bindValue(":value", setting.getValue());
    query.bindValue(":updated_at", setting.getUpdatedAt());
    query.bindValue(":id", setting.getId());

    if (!query.exec())
    {
        qCritical() << "Failed to update setting ID" << setting.getId() << ":" << query.lastError().text();
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    qDebug() << "Successfully updated setting ID:" << id;
    return findById(id);
}
