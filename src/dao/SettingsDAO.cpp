#include "SettingsDAO.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

SettingsDAO::SettingsDAO() : db(DatabaseManager::instance()) {}

QString SettingsDAO::getSetting(const QString& key)
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

bool SettingsDAO::setSetting(const QString& key, const QString& value)
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
