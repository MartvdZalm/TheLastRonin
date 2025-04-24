#include "AppEvents.h"

AppEvents& AppEvents::instance()
{
    static AppEvents instance;
    return instance;
}

void AppEvents::notifyPlaylistChanged()
{
    emit playlistChanged();
}

