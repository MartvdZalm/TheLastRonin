#ifndef IPLAYLISTREPOSITORY_H
#define IPLAYLISTREPOSITORY_H

#include "../model/Playlist.h"
#include "../model/Track.h"
#include <QString>
#include <optional>
#include <vector>

class IPlaylistRepository
{
  public:
    virtual ~IPlaylistRepository() = default;

    virtual std::optional<Playlist> findById(int id) = 0;
    virtual QList<Playlist> findAll() = 0;
    virtual std::optional<Playlist> save(const Playlist& playlist) = 0;
    virtual bool deleteById(int id) = 0;

    virtual bool addTrackToPlaylist(int playlistId, int trackId) = 0;
    virtual bool removeTrackFromPlaylist(int playlistId, int trackId) = 0;
    virtual QList<Track> getTracksForPlaylist(int playlistId) = 0;
};

#endif // IPLAYLISTREPOSITORY_H
