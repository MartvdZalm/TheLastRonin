#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include "../repository/TrackRepository.h"
#include "AudioMetadataService.h"
#include "../model/Track.h"

class TrackService
{
  public:
    std::optional<Track> addTrack(Track& track);

  private:
    AudioMetadataService audioMetadataService;
};

#endif // TRACKSERVICE_H
