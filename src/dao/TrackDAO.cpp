#include "TrackDAO.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

TrackDAO::TrackDAO() : db(DatabaseManager::instance()) {}

bool TrackDAO::insertTrack(int playlistId, const Track& track)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO tracks (title, filePath, artist, album, duration, trackNumber, playlistId)
        VALUES (:title, :filePath, :artist, :album, :duration, :trackNumber, :playlistId)
    )");

    query.bindValue(":title", track.title);
    query.bindValue(":filePath", track.filePath);
    query.bindValue(":artist", track.artist);
    query.bindValue(":album", track.album);
    query.bindValue(":duration", track.duration);
    query.bindValue(":trackNumber", track.trackNumber);
    query.bindValue(":playlistId", playlistId);

    if (!query.exec())
    {
        qDebug() << "Failed to insert track:" << query.lastError();
        return false;
    }

    return true;
}

bool TrackDAO::deleteTrack(int trackId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM tracks WHERE id = :id");
    query.bindValue(":id", trackId);

    if (!query.exec())
    {
        qDebug() << "Failed to delete track:" << query.lastError();
        return false;
    }

    return true;
}

QVector<Track> TrackDAO::getTracksForPlaylist(int playlistId)
{
    QVector<Track> tracks;
    QSqlQuery query;
    query.prepare(
        "SELECT id, title, filePath, artist, album, duration, trackNumber FROM tracks WHERE playlistId = :playlistId");
    query.bindValue(":playlistId", playlistId);

    if (query.exec())
    {
        while (query.next())
        {
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
    }
    else
    {
        qDebug() << "Failed to fetch tracks:" << query.lastError();
    }

    return tracks;
}
