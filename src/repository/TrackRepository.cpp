#include "TrackRepository.h"

#include "../core/Logger.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

TrackRepository::TrackRepository(QSqlDatabase& db) : database(db) {}

std::optional<Track> TrackRepository::findById(int id)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM tracks WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding track by ID " + QString::number(id) + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        return mapFromRecord(query);
    }

    LOG_INFO("Track not found with ID: " + id);
    return std::nullopt;
}

std::optional<Track> TrackRepository::save(const Track& track)
{
    if (track.getId() > 0)
    {
        if (!update(track))
        {
            LOG_ERROR("Failed to update track ID " + track.getId());
            return std::nullopt;
        }

        LOG_INFO("Successfully updated track ID: " + track.getId());
        return track;
    }

    return insert(track);
}

QList<Track> TrackRepository::findAll()
{
    QList<Track> results;
    QSqlQuery query("SELECT * FROM tracks", database);

    if (!query.exec())
    {
        LOG_ERROR("Database error when fetching all tracks: " + query.lastError().text());
        return results;
    }

    while (query.next())
    {
        results.push_back(mapFromRecord(query));
    }

    LOG_INFO("Fetched " + QString::number(results.size()) + " tracks from database");
    return results;
}

bool TrackRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM tracks WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        LOG_ERROR("Failed to delete track ID " + QString::number(id) + " : " + query.lastError().text());
        return false;
    }

    bool deleted = query.numRowsAffected() > 0;
    if (deleted)
    {
        LOG_INFO("Successfully deleted track ID: " + id);
    }
    return deleted;
}

std::optional<Track> TrackRepository::findByFilePath(const QString& filePath)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM tracks WHERE file_path = :file_path");
    query.bindValue(":file_path", filePath);

    if (!query.exec())
    {
        LOG_ERROR("Database error when finding track by path " + filePath + " : " + query.lastError().text());
        return std::nullopt;
    }

    if (query.next())
    {
        return mapFromRecord(query);
    }

    LOG_INFO("Track not found with file path: " + filePath);
    return std::nullopt;
}

Track TrackRepository::mapFromRecord(const QSqlQuery& query)
{
    Track track;
    track.setId(query.value("id").toInt());
    track.setTitle(query.value("title").toString());
    track.setFilePath(query.value("file_path").toString());
    track.setArtist(query.value("artist").toString());
    track.setAlbum(query.value("album").toString());
    track.setDuration(query.value("duration").toInt());
    track.setThumbnailUrl(query.value("thumbnail_url").toString());
    track.setCreatedAt(query.value("created_at").toDateTime());
    track.setUpdatedAt(query.value("updated_at").toDateTime());
    return track;
}

std::optional<Track> TrackRepository::insert(const Track& track)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO tracks (title, file_path, artist, album, duration, thumbnail_url) "
                  "VALUES (:title, :file_path, :artist, :album, :duration, :thumbnail_url)");

    query.bindValue(":title", track.getTitle());
    query.bindValue(":file_path", track.getFilePath());
    query.bindValue(":artist", track.getArtist());
    query.bindValue(":album", track.getAlbum());
    query.bindValue(":duration", track.getDuration());
    query.bindValue(":thumbnail_url", track.getThumbnailUrl());

    if (!query.exec())
    {
        LOG_ERROR("Failed to insert track '" + track.getTitle() + "' (path:" + track.getFilePath() +
                  "): " + query.lastError().text());
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    LOG_INFO("Successfully inserted new track ID: " + id);
    return findById(id);
}

bool TrackRepository::update(const Track& track)
{
    QSqlQuery query(database);
    query.prepare("UPDATE tracks SET title = :title, file_path = :file_path, artist = :artist, album = :album, "
                  "duration = :duration, thumbnail_url = :thumbnail_url, updated_at = "
                  "CURRENT_TIMESTAMP WHERE id = :id");
    query.bindValue(":title", track.getTitle());
    query.bindValue(":file_path", track.getFilePath());
    query.bindValue(":artist", track.getArtist());
    query.bindValue(":album", track.getAlbum());
    query.bindValue(":duration", track.getDuration());
    query.bindValue(":thumbnail_url", track.getThumbnailUrl());
    query.bindValue(":id", track.getId());

    if (!query.exec())
    {
        LOG_ERROR("Failed to update track ID " + QString::number(track.getId()) + " : " + query.lastError().text());
        return false;
    }

    LOG_INFO("Successfully updated track ID: " + track.getId());
    return true;
}
