#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include "../repository/TrackRepository.h"

class TrackService
{
  public:
    TrackService();

    bool addTrackToPlaylist(int playlistId, const Track& track);
    // bool deleteTrack(int trackId);
    // QVector<Track> getTracks(int playlistId);

  private:
    TrackRepository trackRepository;
};

#endif // TRACKSERVICE_H
