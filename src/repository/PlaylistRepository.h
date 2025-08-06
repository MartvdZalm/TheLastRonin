#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H

#include "IPlaylistRepository.h"
#include <QSqlDatabase>

class PlaylistRepository : public IPlaylistRepository
{
  public:
    explicit PlaylistRepository(QSqlDatabase& db);

    std::optional<Playlist> findById(int id) override;
    QList<Playlist> findAll() override;
    std::optional<Playlist> save(const Playlist& playlist) override;
    bool deleteById(int id) override;

    bool addTrackToPlaylist(int playlistId, int trackId) override;
    bool removeTrackFromPlaylist(int playlistId, int trackId) override;
    QList<Track> getTracksForPlaylist(int playlistId) override;

  private:
    QSqlDatabase& database;

    Playlist mapFromRecord(const QSqlQuery& query);
    std::optional<Playlist> insert(const Playlist& playlist);
    bool update(const Playlist& playlist);
};

#endif // PLAYLISTREPOSITORY_H
