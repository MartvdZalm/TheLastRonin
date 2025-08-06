#include "TrackRepository.h"

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
        qCritical() << "Database error when finding track by ID" << id << ":" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next())
    {
        return mapFromRecord(query);
    }

    qDebug() << "Track not found with ID:" << id;
    return std::nullopt;
}

std::optional<Track> TrackRepository::save(const Track& track)
{
    if (track.getId() > 0)
    {
        if (!update(track))
        {
            qCritical() << "Failed to update track ID" << track.getId();
            return std::nullopt;
        }
        return update(track) ? std::optional<Track>(track) : std::nullopt;
    }

    auto existingTrack = findByFilePath(track.getFilePath());
    if (existingTrack)
    {
        qDebug() << "Track already exists with file path:" << track.getFilePath() << "(ID:" << existingTrack->getId()
                 << ")";
        return existingTrack;
    }
    return insert(track);
}

QList<Track> TrackRepository::findAll()
{
    QList<Track> results;
    QSqlQuery query("SELECT * FROM tracks", database);

    if (!query.exec())
    {
        qCritical() << "Database error when fetching all tracks:" << query.lastError().text();
        return results;
    }

    while (query.next())
    {
        results.push_back(mapFromRecord(query));
    }

    qDebug() << "Fetched" << results.size() << "tracks from database";
    return results;
}

bool TrackRepository::deleteById(int id)
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM tracks WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qCritical() << "Failed to delete track ID" << id << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully deleted track ID:" << id;
    return query.exec();
}

std::optional<Track> TrackRepository::findByFilePath(const QString& filePath)
{
    QSqlQuery query(database);
    query.prepare("SELECT * FROM tracks WHERE file_path = :file_path");
    query.bindValue(":file_path", filePath);

    if (!query.exec())
    {
        qCritical() << "Database error when finding track by path" << filePath << ":" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next())
    {
        return mapFromRecord(query);
    }

    qDebug() << "Track not found with file path:" << filePath;
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
    track.setDuration(query.value("duration").toString());
    track.setCreatedAt(query.value("created_at").toDateTime());
    track.setUpdatedAt(query.value("updated_at").toDateTime());
    return track;
}

std::optional<Track> TrackRepository::insert(const Track& track)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO tracks (title, file_path, artist, album, duration) "
                  "VALUES (:title, :file_path, :artist, :album, :duration)");

    query.bindValue(":title", track.getTitle());
    query.bindValue(":file_path", track.getFilePath());
    query.bindValue(":artist", track.getArtist());
    query.bindValue(":album", track.getAlbum());
    query.bindValue(":duration", track.getDuration());

    if (!query.exec())
    {
        qCritical() << "Failed to insert track '" << track.getTitle() << "' (path:" << track.getFilePath()
                    << "):" << query.lastError().text();
        return std::nullopt;
    }

    auto id = query.lastInsertId().toInt();
    qDebug() << "Successfully inserted new track ID:" << id;
    return findById(id);
}

bool TrackRepository::update(const Track& track)
{
    QSqlQuery query(database);
    query.prepare("UPDATE tracks SET title = :title, file_path = :file_path, artist = :artist, album = :album, "
                  "duration = :duration, updated_at = :updated_at WHERE id = :id");
    query.bindValue(":title", track.getTitle());
    query.bindValue(":file_path", track.getFilePath());
    query.bindValue(":artist", track.getArtist());
    query.bindValue(":album", track.getAlbum());
    query.bindValue(":duration", track.getDuration());
    query.bindValue(":updated_at", track.getUpdatedAt());
    query.bindValue(":id", track.getId());

    if (!query.exec())
    {
        qCritical() << "Failed to update track ID" << track.getId() << ":" << query.lastError().text();
        return false;
    }

    qDebug() << "Successfully updated track ID:" << track.getId();
    return true;
}
