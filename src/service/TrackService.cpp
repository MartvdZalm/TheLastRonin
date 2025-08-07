#include "TrackService.h"

#include "../database/Container.h"

std::optional<Track> TrackService::addTrack(Track& track)
{
    auto existingTrack = Container::instance().getTrackRepository()->findByFilePath(track.getFilePath());
    if (existingTrack)
    {
        qDebug() << "Track already exists with file path:" << track.getFilePath() << "(ID:" << existingTrack->getId()
                 << ")";
        return existingTrack;
    }

    auto duration = audioMetadataService.getDuration(track.getFilePath());
    track.setDuration(duration.value_or(0));

    return Container::instance().getTrackRepository()->save(track);
}
