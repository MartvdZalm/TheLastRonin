#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include "../dao/PlaylistDAO.h"
#include "../model/Playlist.h"

class PlaylistService
{
public:
    PlaylistService();

    bool createPlaylist(const Playlist& playlist);
    bool updatePlaylist(const Playlist& playlist);
    bool deletePlaylist(int playlistId);
    QVector<Playlist> getAllPlaylists();
    QVector<Playlist> searchPlaylists(const QString& query);

private:
    PlaylistDAO playlistDAO;
};

#endif // PLAYLISTSERVICE_H
