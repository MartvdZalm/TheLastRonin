#include "TrackService.h"
#include <QDebug>

TrackService::TrackService() {}

bool TrackService::addTrackToPlaylist(int playlistId, const Track& track)
{
    if (track.title.isEmpty() || track.filePath.isEmpty())
    {
        qDebug() << "Track title and file path are required.";
        return false;
    }

    return trackRepository.insertTrack(playlistId, track);
}

bool TrackService::deleteTrack(int trackId)
{
    return trackRepository.deleteTrack(trackId);
}

QVector<Track> TrackService::getTracks(int playlistId)
{
    return trackRepository.getTracksForPlaylist(playlistId);
}
