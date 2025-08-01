#include "SettingsRepository.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

SettingsRepository::SettingsRepository() : db(DatabaseManager::instance()) {}

QString SettingsRepository::getSetting(const QString& key)
{
    QMap<QString, QVariant> bindings;
    bindings[":key"] = key;

    QSqlQuery query;
    query.prepare("SELECT value FROM settings WHERE key = :key");

    for (auto it = bindings.begin(); it != bindings.end(); ++it)
    {
        query.bindValue(it.key(), it.value());
    }

    if (query.exec() && query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

bool SettingsRepository::setSetting(const QString& key, const QString& value)
{
    QMap<QString, QVariant> bindings;
    bindings[":key"] = key;
    bindings[":value"] = value;

    const QString queryStr = R"(
        INSERT OR REPLACE INTO settings (key, value, updatedAt)
        VALUES (:key, :value, CURRENT_TIMESTAMP)
    )";

    return db.executeQuery(queryStr, bindings);
}
