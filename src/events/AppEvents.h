#ifndef APPEVENTS_H
#define APPEVENTS_H

#include "../model/Playlist.h"
#include <QObject>

class AppEvents : public QObject
{
    Q_OBJECT

  public:
    static AppEvents& instance();

    void notifyPlaylistChanged();
    void notifyNavigateToPlaylist(const Playlist& playlist);

  signals:
    void playlistChanged();
    void navigateToPlaylist(const Playlist& playlist);
};

#endif // APPEVENTS_H
