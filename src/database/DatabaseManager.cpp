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

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS albums (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            artist TEXT,
            coverPath TEXT NOT NULL,
            releaseDate DATE NOT NULL
        )
    )")) {
        qDebug() << "Failed to create albums table:" << query.lastError();
        return false;
    }

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

    // Create tracks table
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
    QSqlQuery query("SELECT id, title, artist, coverPath, releaseDate FROM albums");

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

int DatabaseManager::insertPlaylist(const QString& name, const QString& description, const QString& coverImagePath)
{
    QSqlQuery query;
    query.prepare("INSERT INTO playlists (name, description, coverImagePath) VALUES (:name, :description, :coverImagePath)");
    query.bindValue(":name", name);
    query.bindValue(":description", description);
    query.bindValue(":coverImagePath", coverImagePath);

    if (query.exec()) {
        return query.lastInsertId().toInt();
    } else {
        qDebug() << "Insert failed:" << query.lastError();
        return -1;
    }
}

bool DatabaseManager::addTrackToPlaylist(int playlistId, const Track& track)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tracks (title, filePath, artist, album, duration, trackNumber, playlistId) "
                  "VALUES (:title, :filePath, :artist, :album, :duration, :trackNumber, :playlistId)");
    query.bindValue(":title", track.title);
    query.bindValue(":filePath", track.filePath);
    query.bindValue(":artist", track.artist);
    query.bindValue(":album", track.album);
    query.bindValue(":duration", track.duration);
    query.bindValue(":trackNumber", track.trackNumber);
    query.bindValue(":playlistId", playlistId);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Failed to add track to playlist:" << query.lastError();
        return false;
    }
}

QVector<Playlist> DatabaseManager::getAllPlaylists()
{
    QVector<Playlist> playlists;
    QSqlQuery query("SELECT id, name, description, coverImagePath, createdAt, updatedAt FROM playlists");

    while (query.next()) {
        Playlist playlist;
        playlist.id = query.value(0).toInt();
        playlist.name = query.value(1).toString();
        playlist.description = query.value(2).toString();
        playlist.coverImagePath = query.value(3).toString();
        playlist.createdAt = query.value(4).toDateTime();
        playlist.updatedAt = query.value(5).toDateTime();

        playlists.append(playlist);
    }

    return playlists;
}

QVector<Track> DatabaseManager::getTracksForPlaylist(int playlistId)
{
    QVector<Track> tracks;
    QSqlQuery query;
    query.prepare("SELECT id, title, filePath, artist, album, duration, trackNumber FROM tracks WHERE playlistId = :playlistId");
    query.bindValue(":playlistId", playlistId);

    if (query.exec()) {
        while (query.next()) {
            Track track;
            track.id = query.value(0).toInt();
            track.title = query.value(1).toString();
            track.filePath = query.value(2).toString();
            track.artist = query.value(3).toString();
            track.album = query.value(4).toString();
            track.duration = query.value(5).toString();
            track.trackNumber = query.value(6).toInt();

            tracks.append(track);
        }
    } else {
        qDebug() << "Failed to get tracks for playlist:" << query.lastError();
    }

    return tracks;
}
