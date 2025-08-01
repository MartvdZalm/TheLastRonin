#ifndef TRACKREPOSITORY_H
#define TRACKREPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../model/Track.h"
#include <QVector>

class TrackRepository
{
  public:
    TrackRepository();

    bool insertTrack(int playlistId, const Track& track);
    bool deleteTrack(int trackId);
    QVector<Track> getTracksForPlaylist(int playlistId);

  private:
    DatabaseManager& db;
};
#endif // TRACKREPOSITORY_H
