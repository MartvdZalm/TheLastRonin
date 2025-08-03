#include "DatabaseManager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    m_databasePath = path + "/musicplayer.db";

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_databasePath);
}

DatabaseManager::~DatabaseManager()
{
    close();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::open()
{
    if (m_db.isOpen())
        return true;

    if (!m_db.open())
    {
        qCritical() << "Failed to open database:" << m_db.lastError();
        return false;
    }

    // Enable foreign keys and optimize SQLite
    QSqlQuery pragmaQuery(m_db);
    pragmaQuery.exec("PRAGMA foreign_keys = ON");
    pragmaQuery.exec("PRAGMA journal_mode = WAL");
    pragmaQuery.exec("PRAGMA synchronous = NORMAL");

    return true;
}

void DatabaseManager::close()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

QString DatabaseManager::databasePath() const
{
    return m_databasePath;
}

QSqlQuery DatabaseManager::executeQuery(const QString& queryStr, const QMap<QString, QVariant>& bindings)
{
    QSqlQuery query(m_db);

    if (!query.prepare(queryStr))
    {
        qCritical() << "Failed to prepare query:" << query.lastError();
        return query;
    }

    for (auto it = bindings.constBegin(); it != bindings.constEnd(); ++it)
    {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec())
    {
        qCritical() << "Query failed:" << query.lastError();
    }

    return query;
}

bool DatabaseManager::execute(const QString& queryStr, const QMap<QString, QVariant>& bindings)
{
    QSqlQuery query = executeQuery(queryStr, bindings);
    return query.isActive();
}

bool DatabaseManager::beginTransaction()
{
    if (!m_db.transaction())
    {
        qCritical() << "Failed to begin transaction:" << m_db.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::commitTransaction()
{
    if (!m_db.commit())
    {
        qCritical() << "Failed to commit transaction:" << m_db.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::rollbackTransaction()
{
    if (!m_db.rollback())
    {
        qCritical() << "Failed to rollback transaction:" << m_db.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::tableExists(const QString& tableName)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=:tableName");
    query.bindValue(":tableName", tableName);

    return query.exec() && query.next();
}
