#include "DatabaseManager.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

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
    if (!db.open()) {
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
        )")) {
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
        )")) {
            qDebug() << "Failed to create tracks table:" << query.lastError();
            return false;
        }

       return true;
}

bool DatabaseManager::executeQuery(const QString& queryStr, const QMap<QString, QVariant>& bindings)
{
    QSqlQuery query;
    query.prepare(queryStr);

    for (auto it = bindings.begin(); it != bindings.end(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError();
        return false;
    }
    return true;
}

