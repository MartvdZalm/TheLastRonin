#include "PlaylistRepository.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

PlaylistRepository::PlaylistRepository(QSqlDatabase& db) : database(db) {}

std::optional<Playlist> PlaylistRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM playlists WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qCritical() << "Database error when finding playlist ID" << id << ":" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next())
    {
        auto playlist = mapFromRecord(query);
        playlist.setTracks(getTracksForPlaylist(id));
        qDebug() << "Successfully found playlist ID:" << id;
        return playlist;
    }

    qDebug() << "Playlist not found with ID:" << id;
    return std::nullopt;
}

std::optional<Playlist> PlaylistRepository::save(const Playlist& playlist)
{
    if (playlist.getId() > 0)
    {
        if (!update(playlist))
        {
            qCritical() << "Failed to update playlist ID" << playlist.getId();
            return std::nullopt;
        }
        qDebug() << "Successfully updated playlist ID:" << playlist.getId();
        return playlist;
    }

    return insert(playlist);
}

QList<Playlist> PlaylistRepository::findAll()
{
    QList<Playlist> results;
    QSqlQuery query("SELECT * FROM playlists", database);

    if (!query.exec())
    {
        qCritical() << "Database error when fetching all playlists:" << query.lastError().text();
        return results;
    }

    while (query.next())
    {
        results.append(mapFromRecord(query));
    }

    qDebug() << "Fetched" << results.size() << "playlists from database";
    return results;
}

bool PlaylistRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM playlists WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qCritical() << "Failed to delete playlist ID" << id << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully deleted playlist ID:" << id;
    return query.exec();
}

bool PlaylistRepository::addTrackToPlaylist(int playlistId, int trackId)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO playlists_tracks (playlist_id, track_id) "
                  "VALUES (:playlistId, :trackId)");

    query.bindValue(":playlistId", playlistId);
    query.bindValue(":trackId", trackId);

    if (!query.exec())
    {
        qCritical() << "Failed to add track ID" << trackId << "to playlist ID" << playlistId << ":"
                    << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully added track ID" << trackId << "to playlist ID" << playlistId;
    return true;
}

bool PlaylistRepository::removeTrackFromPlaylist(int playlistId, int trackId)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM playlists_tracks "
                  "WHERE playlist_id = :playlistId AND track_id = :trackId");
    query.bindValue(":playlistId", playlistId);
    query.bindValue(":trackId", trackId);

    if (!query.exec())
    {
        qCritical() << "Failed to remove track ID" << trackId << "from playlist ID" << playlistId << ":"
                    << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0)
    {
        qDebug() << "Successfully removed track ID" << trackId << "from playlist ID" << playlistId;
        return true;
    }

    return false;
}

QList<Track> PlaylistRepository::getTracksForPlaylist(int playlistId)
{
    QList<Track> tracks;
    QSqlQuery query(database);
    query.prepare("SELECT t.* FROM tracks t "
                  "INNER JOIN playlists_tracks pt ON t.id = pt.track_id "
                  "WHERE pt.playlist_id = :playlistId "
                  "ORDER BY pt.added_at");
    query.bindValue(":playlistId", playlistId);

    if (!query.exec())
    {
        qCritical() << "Database error when fetching tracks for playlist ID" << playlistId << ":"
                    << query.lastError().text();
        return tracks;
    }

    while (query.next())
    {
        Track track;
        track.setId(query.value("id").toInt());
        track.setTitle(query.value("title").toString());
        track.setFilePath(query.value("file_path").toString());
        track.setArtist(query.value("artist").toString());
        track.setAlbum(query.value("album").toString());
        track.setDuration(query.value("duration").toString());
        tracks.append(track);
    }

    qDebug() << "Fetched" << tracks.size() << "tracks for playlist ID:" << playlistId;
    return tracks;
}

Playlist PlaylistRepository::mapFromRecord(const QSqlQuery& query)
{
    Playlist playlist;
    playlist.setId(query.value("id").toInt());
    playlist.setName(query.value("name").toString());
    playlist.setDescription(query.value("description").toString());
    playlist.setCoverImagePath(query.value("cover_image_path").toString());
    playlist.setCreatedAt(query.value("created_at").toDateTime());
    playlist.setUpdatedAt(query.value("updated_at").toDateTime());
    return playlist;
}

std::optional<Playlist> PlaylistRepository::insert(const Playlist& playlist)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO playlists (name, description, cover_image_path) "
                  "VALUES (:name, :description, :cover_image_path)");

    query.bindValue(":name", playlist.getName());
    query.bindValue(":description", playlist.getDescription());
    query.bindValue(":cover_image_path", playlist.getCoverImagePath());

    if (!query.exec())
    {
        qCritical() << "Failed to insert playlist '" << playlist.getName() << "':" << query.lastError().text();
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    qDebug() << "Successfully inserted new playlist ID:" << id;
    return findById(id);
}

bool PlaylistRepository::update(const Playlist& playlist)
{
    QSqlQuery query(database);
    query.prepare(
        "UPDATE playlists "
        "SET name = :name, description = :description, cover_image_path = :cover_image_path, updated_at = :updated_at "
        "WHERE id = :id");

    query.bindValue(":name", playlist.getName());
    query.bindValue(":description", playlist.getDescription());
    query.bindValue(":cover_image_path", playlist.getCoverImagePath());
    query.bindValue(":updated_at", playlist.getUpdatedAt());
    query.bindValue(":id", playlist.getId());

    if (!query.exec())
    {
        qCritical() << "Failed to update playlist ID" << playlist.getId() << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully updated playlist ID:" << playlist.getId();
    return true;
}
