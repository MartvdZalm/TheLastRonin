#ifndef TRACKDAO_H
#define TRACKDAO_H

#include "../database/DatabaseManager.h"
#include "../model/Track.h"
#include <QVector>

class TrackDAO
{
  public:
    TrackDAO();

    bool insertTrack(int playlistId, const Track& track);
    bool deleteTrack(int trackId);
    QVector<Track> getTracksForPlaylist(int playlistId);

  private:
    DatabaseManager& db;
};
#endif // TRACKDAO_H
