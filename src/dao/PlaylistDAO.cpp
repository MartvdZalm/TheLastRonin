#include "PlaylistDAO.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

PlaylistDAO::PlaylistDAO() : db(DatabaseManager::instance()) {}

int PlaylistDAO::insertPlaylist(const Playlist& playlist)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO playlists (name, description, coverImagePath)
        VALUES (:name, :description, :coverImagePath)
    )");

    query.bindValue(":name", playlist.name);
    query.bindValue(":description", playlist.description);
    query.bindValue(":coverImagePath", playlist.coverImagePath);

    if (!query.exec())
    {
        qWarning() << "Failed to insert playlist:" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool PlaylistDAO::updatePlaylist(const Playlist& playlist)
{
    QMap<QString, QVariant> bindings;
    bindings[":name"] = playlist.name;
    bindings[":description"] = playlist.description;
    bindings[":coverImagePath"] = playlist.coverImagePath;
    bindings[":id"] = playlist.id;

    const QString queryStr = R"(
        UPDATE playlists
        SET name = :name,
            description = :description,
            coverImagePath = :coverImagePath,
            updatedAt = CURRENT_TIMESTAMP
        WHERE id = :id
    )";

    return db.executeQuery(queryStr, bindings);
}

bool PlaylistDAO::deletePlaylist(int playlistId)
{
    QMap<QString, QVariant> bindings;
    bindings[":id"] = playlistId;

    const QString queryStr = R"(
        DELETE FROM playlists
        WHERE id = :id
    )";

    return db.executeQuery(queryStr, bindings);
}

QVector<Playlist> PlaylistDAO::getAllPlaylists()
{
    QVector<Playlist> playlists;
    QMap<int, Playlist> playlistMap;

    QSqlQuery query(R"(
        SELECT
            p.id, p.name, p.description, p.coverImagePath, p.createdAt, p.updatedAt,
            t.id, t.title, t.filePath, t.artist, t.album, t.duration, t.trackNumber
        FROM playlists p
        LEFT JOIN tracks t ON t.playlistId = p.id
        ORDER BY p.id, t.trackNumber
    )");

    while (query.next())
    {
        int playlistId = query.value(0).toInt();

        if (!playlistMap.contains(playlistId))
        {
            Playlist playlist;
            playlist.id = playlistId;
            playlist.name = query.value(1).toString();
            playlist.description = query.value(2).toString();
            playlist.coverImagePath = query.value(3).toString();
            playlist.createdAt = query.value(4).toDateTime();
            playlist.updatedAt = query.value(5).toDateTime();
            playlistMap[playlistId] = playlist;
        }

        if (!query.value(6).isNull())
        {
            Track track;
            track.id = query.value(6).toInt();
            track.title = query.value(7).toString();
            track.filePath = query.value(8).toString();
            track.artist = query.value(9).toString();
            track.album = query.value(10).toString();
            track.duration = query.value(11).toString();
            track.trackNumber = query.value(12).toInt();
            playlistMap[playlistId].tracks.append(track);
        }
    }

    return playlistMap.values().toVector();
}

QVector<Playlist> PlaylistDAO::searchPlaylists(const QString& query)
{
    QVector<Playlist> result;
    QMap<QString, QVariant> bindings;
    bindings[":query"] = "%" + query + "%";

    const QString queryStr = R"(
        SELECT id, name, description, coverImagePath
        FROM playlists
        WHERE name LIKE :query
    )";

    QSqlQuery dbQuery;
    dbQuery.prepare(queryStr);
    for (auto it = bindings.begin(); it != bindings.end(); ++it)
    {
        dbQuery.bindValue(it.key(), it.value());
    }

    if (dbQuery.exec())
    {
        while (dbQuery.next())
        {
            Playlist p;
            p.id = dbQuery.value(0).toInt();
            p.name = dbQuery.value(1).toString();
            p.description = dbQuery.value(2).toString();
            p.coverImagePath = dbQuery.value(3).toString();
            result.append(p);
        }
    }
    else
    {
        qDebug() << "Failed to search playlists:" << dbQuery.lastError();
    }

    return result;
}
