#include "Container.h"

#include "../repository/PlaylistRepository.h"
#include "../repository/SettingRepository.h"
#include "../repository/TrackRepository.h"

Container& Container::instance()
{
    static Container instance;
    return instance;
}

void Container::initialize(QSqlDatabase& database)
{
    this->database = &database;
}

std::shared_ptr<ISettingRepository> Container::getSettingRepository()
{
    if (!settingRepository && database)
    {
        settingRepository = std::make_shared<SettingRepository>(*database);
    }
    return settingRepository;
}

std::shared_ptr<IPlaylistRepository> Container::getPlaylistRepository()
{
    if (!playlistRepository && database)
    {
        playlistRepository = std::make_shared<PlaylistRepository>(*database);
    }
    return playlistRepository;
}

std::shared_ptr<ITrackRepository> Container::getTrackRepository()
{
    if (!trackRepository && database)
    {
        trackRepository = std::make_shared<TrackRepository>(*database);
    }
    return trackRepository;
}
