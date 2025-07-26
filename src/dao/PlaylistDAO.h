#ifndef PLAYLISTDAO_H
#define PLAYLISTDAO_H

#include "../database/DatabaseManager.h"
#include "../model/Playlist.h"
#include <QMap>
#include <QString>
#include <QVariant>
#include <QVector>

class PlaylistDAO
{
  public:
    PlaylistDAO();

    int insertPlaylist(const Playlist& playlist);
    bool updatePlaylist(const Playlist& playlist);
    bool deletePlaylist(int playlistId);
    QVector<Playlist> getAllPlaylists();
    QVector<Playlist> searchPlaylists(const QString& query);

  private:
    DatabaseManager& db;
};

#endif // PLAYLISTDAO_H
