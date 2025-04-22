#ifndef PLAYLISTDAO_H
#define PLAYLISTDAO_H

#include <QVector>
#include <QString>
#include <QVariant>
#include <QMap>
#include "../database/DatabaseManager.h"
#include "../model/Playlist.h"

class PlaylistDAO
{
public:
    PlaylistDAO();

    bool insertPlaylist(const Playlist& playlist);
    bool updatePlaylist(const Playlist& playlist);
    bool deletePlaylist(int playlistId);
    QVector<Playlist> getAllPlaylists();
    QVector<Playlist> searchPlaylists(const QString& query);

private:
    DatabaseManager& db;
};

#endif // PLAYLISTDAO_H
