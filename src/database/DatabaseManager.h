#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include "../album/Album.h"
#include "../playlist/Playlist.h"
#include "../playlist/Track.h"

class DatabaseManager
{
public:
    DatabaseManager();

    static DatabaseManager& instance();

    bool openDatabase();
    bool initSchema();

    int insertAlbum(const QString& title, const QString& artist, const QString& coverPath, const QString& releaseDate);
    QVector<Album> getAllAlbums();

    int insertPlaylist(const Playlist& playlist);
    bool addTrackToPlaylist(int playlistId, const Track& track);
    QVector<Playlist> getAllPlaylists();
    QVector<Track> getTracksForPlaylist(int playlistId);
    QVector<Playlist> searchPlaylists(const QString& query);

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
