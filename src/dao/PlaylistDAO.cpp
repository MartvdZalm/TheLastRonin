#include "PlaylistDAO.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PlaylistDAO::PlaylistDAO() : db(DatabaseManager::instance()) {}

bool PlaylistDAO::insertPlaylist(const Playlist& playlist)
{
    QMap<QString, QVariant> bindings;
    bindings[":name"] = playlist.name;
    bindings[":description"] = playlist.description;
    bindings[":coverImagePath"] = playlist.coverImagePath;

    const QString queryStr = R"(
        INSERT INTO playlists (name, description, coverImagePath)
        VALUES (:name, :description, :coverImagePath)
    )";

    return db.executeQuery(queryStr, bindings);
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
    for (auto it = bindings.begin(); it != bindings.end(); ++it) {
        dbQuery.bindValue(it.key(), it.value());
    }

    if (dbQuery.exec()) {
        while (dbQuery.next()) {
            Playlist p;
            p.id = dbQuery.value(0).toInt();
            p.name = dbQuery.value(1).toString();
            p.description = dbQuery.value(2).toString();
            p.coverImagePath = dbQuery.value(3).toString();
            result.append(p);
        }
    } else {
        qDebug() << "Failed to search playlists:" << dbQuery.lastError();
    }

    return result;
}
