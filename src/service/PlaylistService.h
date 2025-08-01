#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include "../model/Playlist.h"
#include "../repository/PlaylistRepository.h"

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
    PlaylistRepository playlistRepository;
};

#endif // PLAYLISTSERVICE_H
