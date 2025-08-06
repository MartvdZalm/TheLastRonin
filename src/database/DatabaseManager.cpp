#include "DatabaseManager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

QMutex DatabaseManager::s_instanceMutex;

DatabaseManager& DatabaseManager::instance()
{
    QMutexLocker locker(&s_instanceMutex);
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DatabaseManager::initialize()
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized)
        return true;

    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/musicplayer.db";
    QDir().mkpath(QFileInfo(dbPath).absolutePath());

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        qCritical() << "Database error:" << m_db.lastError();
        return false;
    }

    if (!applyPragmas())
    {
        qCritical() << "Failed to apply database optimizations";
        return false;
    }

    if (!createTables())
    {
        qCritical() << "Failed to create database tables";
        return false;
    }

    if (!verifyDatabase())
    {
        qCritical() << "Database verification failed";
        return false;
    }

    m_initialized = true;
    return true;
}

bool DatabaseManager::applyPragmas()
{
    QSqlQuery query(m_db);
    return query.exec("PRAGMA foreign_keys = ON") && query.exec("PRAGMA journal_mode = WAL") &&
           query.exec("PRAGMA synchronous = NORMAL") && query.exec("PRAGMA temp_store = MEMORY");
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_db);

    if (!query.exec("BEGIN IMMEDIATE TRANSACTION"))
    {
        qCritical() << "Failed to begin transaction:" << query.lastError();
        return false;
    }

    const QStringList tables = {
        R"(
            CREATE TABLE IF NOT EXISTS playlists (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                description TEXT,
                cover_image_path TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )",
        R"(
            CREATE TABLE IF NOT EXISTS tracks (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                file_path TEXT NOT NULL UNIQUE,
                artist TEXT,
                album TEXT,
                duration TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )",
        R"(
            CREATE TABLE IF NOT EXISTS playlists_tracks (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                playlist_id INTEGER NOT NULL,
                track_id INTEGER NOT NULL,
                added_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )",
        R"(
            CREATE TABLE IF NOT EXISTS settings (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                key TEXT NOT NULL UNIQUE,
                value TEXT,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )"};

    for (const auto& table : tables)
    {
        if (!query.exec(table))
        {
            qCritical() << "Table creation failed:" << query.lastError();
            query.exec("ROLLBACK");
            return false;
        }
    }

    if (!query.exec("COMMIT"))
    {
        qCritical() << "Commit failed:" << query.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::verifyDatabase()
{
    QSqlQuery query(m_db);
    return query.exec("PRAGMA quick_check");
}

bool DatabaseManager::execute(const QString& query, const QVariantMap& params)
{
    QMutexLocker locker(&m_mutex);
    QSqlQuery q(m_db);

    q.prepare(query);
    for (auto it = params.constBegin(); it != params.constEnd(); ++it)
    {
        q.bindValue(":" + it.key(), it.value());
    }

    if (!q.exec())
    {
        qWarning() << "Query failed:" << query << "\nError:" << q.lastError();
        return false;
    }
    return true;
}
