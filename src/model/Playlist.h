#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Track.h"
#include <QDateTime>

struct Playlist
{
    int id;
    QString name;
    QString description;
    QString coverImagePath;
    QVector<Track> tracks;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // PLAYLIST_H
