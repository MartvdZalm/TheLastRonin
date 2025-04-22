#ifndef TRACK_H
#define TRACK_H

#include <QString>

struct Track
{
    int id;
    QString title;
    QString filePath;
    QString artist;
    QString album;
    QString duration;
    int trackNumber;
};

#endif // TRACK_H
