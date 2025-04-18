#include "DatabaseManager.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlError>

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

bool DatabaseManager::initSchema()
{
    QSqlQuery query;
    return query.exec(R"(
        CREATE TABLE IF NOT EXISTS albums (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            artist TEXT,
            coverPath TEXT NOT NULL,
            releaseDate DATE NOT NULL
        )
    )");
}


int DatabaseManager::insertAlbum(
    const QString& title,
    const QString& artist,
    const QString& coverPath,
    const QString& releaseDate
)
{
    QSqlQuery query;
    query.prepare("INSERT INTO albums (title, artist, coverPath, releaseDate) VALUES (:title, :artist, :coverPath, :releaseDate)");
    query.bindValue(":title", title);
    query.bindValue(":artist", artist);
    query.bindValue(":coverPath", coverPath);
    query.bindValue(":releaseDate", releaseDate);

    if (query.exec()) {
        return query.lastInsertId().toInt();
    } else {
        qDebug() << "Insert failed:" << query.lastError();
        return -1;
    }
}

QVector<Album> DatabaseManager::getAllAlbums()
{
    QVector<Album> albums;
    QSqlQuery query("SELECT id, title, artist, releaseDate coverPath FROM albums");

    while (query.next()) {
        Album album;
        album.id = query.value(0).toInt();
        album.title = query.value(1).toString();
        album.artist = query.value(2).toString();
        album.coverPath = query.value(3).toString();
        album.releaseDate = query.value(4).toString();
        albums.append(album);
    }

    return albums;
}
