#include "PlaylistService.h"
#include <QDebug>

PlaylistService::PlaylistService() {}

bool PlaylistService::createPlaylist(const Playlist& playlist)
{
    if (playlist.name.isEmpty()) {
        qDebug() << "Playlist name cannot be empty!";
        return false;
    }

    return playlistDAO.insertPlaylist(playlist);
}

bool PlaylistService::updatePlaylist(const Playlist& playlist)
{
    if (playlist.name.isEmpty()) {
        qDebug() << "Playlist name cannot be empty!";
        return false;
    }

    return playlistDAO.updatePlaylist(playlist);
}

bool PlaylistService::deletePlaylist(int playlistId)
{
    return playlistDAO.deletePlaylist(playlistId);
}

QVector<Playlist> PlaylistService::getAllPlaylists()
{
    return playlistDAO.getAllPlaylists();
}

QVector<Playlist> PlaylistService::searchPlaylists(const QString& query)
{
    return playlistDAO.searchPlaylists(query);
}
