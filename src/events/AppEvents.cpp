#include "AppEvents.h"

AppEvents& AppEvents::instance()
{
    static AppEvents instance;
    return instance;
}

void AppEvents::notifyNavigateToHome()
{
    emit navigateToHome();
}

void AppEvents::notifyPlaylistChanged()
{
    emit playlistChanged();
}

void AppEvents::notifyNavigateToPlaylist(const Playlist& playlist)
{
    emit navigateToPlaylist(playlist);
}

void AppEvents::notifyNavigateToSettings()
{
    emit navigateToSettings();
}
