#ifndef APPEVENTS_H
#define APPEVENTS_H

#include <QObject>

class AppEvents : public QObject
{
    Q_OBJECT

public:
    static AppEvents& instance();

    void notifyPlaylistChanged();

signals:
    void playlistChanged();
};

#endif // APPEVENTS_H
