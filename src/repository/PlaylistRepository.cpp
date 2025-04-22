#include "PlaylistRepository.h"

QVector<Playlist> PlaylistRepository::getAll()
{
    return db.getAllPlaylists();
}

QVector<Playlist> PlaylistRepository::search(const QString& query)
{
    return db.searchPlaylists(query);
}

QVector<Track> PlaylistRepository::getTracksForPlaylist(int playlistId)
{
    return db.getTracksForPlaylist(playlistId);
}

int PlaylistRepository::insert(const Playlist& playlist)
{
    return db.insertPlaylist(playlist);
}

bool PlaylistRepository::update(const Playlist& playlist)
{
    return db.updatePlaylist(playlist);
}

bool PlaylistRepository::remove(int playlistId)
{
    return db.deletePlaylist(playlistId);
}

bool PlaylistRepository::addTrackToPlaylist(int playlistId, const Track& track)
{
    return db.addTrackToPlaylist(playlistId, track);
}

