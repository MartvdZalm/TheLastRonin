#include "DatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

DatabaseManager::DatabaseManager()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QString dbPath = path + "/musicplayer.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDatabase()
{
    if (!db.open())
    {
        qDebug() << "Failed to open database:" << db.lastError();
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    db.close();
}

bool DatabaseManager::initSchema()
{
    QSqlQuery query;

    if (!query.exec(R"(
            CREATE TABLE IF NOT EXISTS playlists (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                description TEXT,
                coverImagePath TEXT,
                createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
                updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP
            )
        )"))
    {
        qDebug() << "Failed to create playlists table:" << query.lastError();
        return false;
    }

    if (!query.exec(R"(
            CREATE TABLE IF NOT EXISTS tracks (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                filePath TEXT NOT NULL,
                artist TEXT,
                album TEXT,
                duration TEXT,
                trackNumber INTEGER,
                playlistId INTEGER,
                FOREIGN KEY (playlistId) REFERENCES playlists(id)
            )
        )"))
    {
        qDebug() << "Failed to create tracks table:" << query.lastError();
        return false;
    }

    if (!query.exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                email TEXT NOT NULL,
                password TEXT NOT NULL,
                client_id TEXT NOT NULL
            )
        )"))
    {
        qDebug() << "Failed to create users table:" << query.lastError();
        return false;
    }

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS settings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            key TEXT NOT NULL UNIQUE,
            value TEXT,
            createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
            updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )"))
    {
        qDebug() << "Failed to create settings table:" << query.lastError();
        return false;
    }

    if (!query.exec("SELECT COUNT(*) FROM settings"))
    {
        qDebug() << "Failed to check settings count:" << query.lastError();
        return false;
    }

    query.next();
    int settingsCount = query.value(0).toInt();

    if (settingsCount == 0)
    {
        if (!query.exec("INSERT INTO settings (key, value) VALUES ('language', 'English')"))
        {
            qDebug() << "Failed to insert default language setting:" << query.lastError();
        }
    }

    return true;
}

bool DatabaseManager::deleteUserData()
{
    closeDatabase();

    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/musicplayer.db";
    if (QFile::exists(dbPath))
    {
        if (QFile::remove(dbPath))
        {
            qDebug() << "User data deleted successfully.";
            return true;
        }
        else
        {
            qDebug() << "Failed to delete database file.";
            return false;
        }
    }
    else
    {
        qDebug() << "Database file does not exist.";
        return true;
    }
}

bool DatabaseManager::executeQuery(const QString& queryStr, const QMap<QString, QVariant>& bindings)
{
    QSqlQuery query;
    query.prepare(queryStr);

    for (auto it = bindings.begin(); it != bindings.end(); ++it)
    {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec())
    {
        qDebug() << "Query failed:" << query.lastError();
        return false;
    }
    return true;
}
