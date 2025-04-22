#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../playlist/Playlist.h"
#include "../playlist/Track.h"

class PlaylistRepository
{
public:
    PlaylistRepository(DatabaseManager& db): db(db) { }

    QVector<Playlist> getAll();
    QVector<Playlist> search(const QString& query);
    QVector<Track> getTracksForPlaylist(int playlistId);
    int insert(const Playlist& playlist);
    bool update(const Playlist& playlist);
    bool addTrackToPlaylist(int playlistId, const Track& track);

private:
    DatabaseManager& db;
};

#endif // PLAYLISTREPOSITORY_H
