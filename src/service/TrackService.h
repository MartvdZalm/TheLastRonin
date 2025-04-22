#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include "../dao/TrackDAO.h"

class TrackService
{
public:
    TrackService();

    bool addTrackToPlaylist(int playlistId, const Track& track);
    bool deleteTrack(int trackId);
    QVector<Track> getTracks(int playlistId);

private:
    TrackDAO trackDao;
};

#endif // TRACKSERVICE_H
