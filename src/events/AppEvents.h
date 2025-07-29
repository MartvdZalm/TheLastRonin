#ifndef APPEVENTS_H
#define APPEVENTS_H

#include "../model/Playlist.h"
#include <QObject>

class AppEvents : public QObject
{
    Q_OBJECT

  public:
    static AppEvents& instance();

    void notifyNavigateToHome();
    void notifyPlaylistChanged();
    void notifyNavigateToPlaylist(const Playlist& playlist);
    void notifyNavigateToSettings();

  signals:
    void navigateToHome();
    void playlistChanged();
    void navigateToPlaylist(const Playlist& playlist);
    void navigateToSettings();
};

#endif // APPEVENTS_H
