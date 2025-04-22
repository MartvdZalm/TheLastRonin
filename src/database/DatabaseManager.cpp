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

// DatabaseManager::DatabaseManager()
// {
//     QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     QDir().mkpath(path);
//     QString dbPath = path + "/musicplayer.db";

//     db = QSqlDatabase::addDatabase("QSQLITE");
//     db.setDatabaseName(dbPath);
// }

// DatabaseManager& DatabaseManager::instance()
// {
//     static DatabaseManager instance;
//     return instance;
// }

// bool DatabaseManager::openDatabase()
// {
//     if (!db.open()) {
//         qDebug() << "Failed to open database:" << db.lastError();
//         return false;
//     }
//     return true;
// }

// bool DatabaseManager::initSchema()
// {
//     QSqlQuery query;

//     if (!query.exec(R"(
//         CREATE TABLE IF NOT EXISTS playlists (
//             id INTEGER PRIMARY KEY AUTOINCREMENT,
//             name TEXT NOT NULL,
//             description TEXT,
//             coverImagePath TEXT,
//             createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
//             updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP
//         )
//     )")) {
//         qDebug() << "Failed to create playlists table:" << query.lastError();
//         return false;
//     }

//     if (!query.exec(R"(
//         CREATE TABLE IF NOT EXISTS tracks (
//             id INTEGER PRIMARY KEY AUTOINCREMENT,
//             title TEXT NOT NULL,
//             filePath TEXT NOT NULL,
//             artist TEXT,
//             album TEXT,
//             duration TEXT,
//             trackNumber INTEGER,
//             playlistId INTEGER,
//             FOREIGN KEY (playlistId) REFERENCES playlists(id)
//         )
//     )")) {
//         qDebug() << "Failed to create tracks table:" << query.lastError();
//         return false;
//     }

//    return true;
// }

// int DatabaseManager::insertPlaylist(const Playlist& playlist)
// {
//     QSqlQuery query;
//     query.prepare("INSERT INTO playlists (name, description, coverImagePath) VALUES (:name, :description, :coverImagePath)");
//     query.bindValue(":name", playlist.name);
//     query.bindValue(":description", playlist.description);
//     query.bindValue(":coverImagePath", playlist.coverImagePath);

//     if (query.exec()) {
//         return query.lastInsertId().toInt();
//     } else {
//         qDebug() << "Insert failed:" << query.lastError();
//         return -1;
//     }
// }

// bool DatabaseManager::updatePlaylist(const Playlist& playlist)
// {
//     QSqlQuery query;
//     query.prepare(R"(
//         UPDATE playlists
//         SET name = :name,
//             description = :description,
//             coverImagePath = :coverImagePath,
//             updatedAt = CURRENT_TIMESTAMP
//         WHERE id = :id
//     )");
//     query.bindValue(":name", playlist.name);
//     query.bindValue(":description", playlist.description);
//     query.bindValue(":coverImagePath", playlist.coverImagePath);
//     query.bindValue(":id", playlist.id);

//     if (!query.exec()) {
//         qDebug() << "Failed to update playlist:" << query.lastError();
//         return false;
//     }

//     return true;
// }

// bool DatabaseManager::deletePlaylist(int playlistId)
// {
//     QSqlQuery query;
//     query.prepare(R"(
//         DELETE FROM playlists
//         WHERE id = :id
//     )");
//     query.bindValue(":id", playlistId);

//     if (!query.exec()) {
//         qDebug() << "Failed to delete playlist: " << query.lastError();
//         return false;
//     }

//     return true;
// }

// bool DatabaseManager::addTrackToPlaylist(int playlistId, const Track& track)
// {
//     QSqlQuery query;
//     query.prepare("INSERT INTO tracks (title, filePath, artist, album, duration, trackNumber, playlistId) "
//                   "VALUES (:title, :filePath, :artist, :album, :duration, :trackNumber, :playlistId)");
//     query.bindValue(":title", track.title);
//     query.bindValue(":filePath", track.filePath);
//     query.bindValue(":artist", track.artist);
//     query.bindValue(":album", track.album);
//     query.bindValue(":duration", track.duration);
//     query.bindValue(":trackNumber", track.trackNumber);
//     query.bindValue(":playlistId", playlistId);

//     if (query.exec()) {
//         qDebug() << "Query:" << query.lastQuery();
//         qDebug() << "Bound values:"
//                  << query.boundValue(":title") << query.boundValue(":filePath")
//                  << query.boundValue(":artist") << query.boundValue(":album")
//                  << query.boundValue(":duration") << query.boundValue(":trackNumber")
//                  << query.boundValue(":playlistId");

//         return true;
//     } else {
//         qDebug() << "Failed to add track to playlist:" << query.lastError();
//         return false;
//     }
// }

// QVector<Playlist> DatabaseManager::getAllPlaylists()
// {
//     QVector<Playlist> playlists;
//     QSqlQuery query("SELECT id, name, description, coverImagePath, createdAt, updatedAt FROM playlists");

//     while (query.next()) {
//         Playlist playlist;
//         playlist.id = query.value(0).toInt();
//         playlist.name = query.value(1).toString();
//         playlist.description = query.value(2).toString();
//         playlist.coverImagePath = query.value(3).toString();
//         playlist.createdAt = query.value(4).toDateTime();
//         playlist.updatedAt = query.value(5).toDateTime();
//         playlist.tracks = getTracksForPlaylist(playlist.id);

//         playlists.append(playlist);
//     }

//     return playlists;
// }

// QVector<Track> DatabaseManager::getTracksForPlaylist(int playlistId)
// {
//     QVector<Track> tracks;
//     QSqlQuery query;
//     query.prepare("SELECT id, title, filePath, artist, album, duration, trackNumber FROM tracks WHERE playlistId = :playlistId");
//     query.bindValue(":playlistId", playlistId);

//     if (query.exec()) {
//         while (query.next()) {
//             Track track;
//             track.id = query.value(0).toInt();
//             track.title = query.value(1).toString();
//             track.filePath = query.value(2).toString();
//             track.artist = query.value(3).toString();
//             track.album = query.value(4).toString();
//             track.duration = query.value(5).toString();
//             track.trackNumber = query.value(6).toInt();

//             tracks.append(track);
//         }
//     } else {
//         qDebug() << "Failed to get tracks for playlist:" << query.lastError();
//     }

//     return tracks;
// }

// QVector<Playlist> DatabaseManager::searchPlaylists(const QString& query)
// {
//     QVector<Playlist> result;
//     QSqlQuery q;
//     q.prepare("SELECT id, name, description, coverImagePath FROM playlists WHERE name LIKE :query");
//     q.bindValue(":query", "%" + query + "%");

//     if (q.exec()) {
//         while (q.next()) {
//             Playlist p;
//             p.id = q.value(0).toInt();
//             p.name = q.value(1).toString();
//             p.description = q.value(2).toString();
//             p.coverImagePath = q.value(3).toString();
//             result.append(p);
//         }
//     }

//     return result;
// }

