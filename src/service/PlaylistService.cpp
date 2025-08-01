#include "PlaylistService.h"
#include <QDebug>

PlaylistService::PlaylistService() {}

bool PlaylistService::createPlaylist(const Playlist& playlist)
{
    if (playlist.name.isEmpty())
    {
        qDebug() << "Playlist name cannot be empty!";
        return false;
    }

    return playlistRepository.insertPlaylist(playlist);
}

bool PlaylistService::updatePlaylist(const Playlist& playlist)
{
    if (playlist.name.isEmpty())
    {
        qDebug() << "Playlist name cannot be empty!";
        return false;
    }

    return playlistRepository.updatePlaylist(playlist);
}

bool PlaylistService::deletePlaylist(int playlistId)
{
    return playlistRepository.deletePlaylist(playlistId);
}

QVector<Playlist> PlaylistService::getAllPlaylists()
{
    return playlistRepository.getAllPlaylists();
}

QVector<Playlist> PlaylistService::searchPlaylists(const QString& query)
{
    return playlistRepository.searchPlaylists(query);
}
