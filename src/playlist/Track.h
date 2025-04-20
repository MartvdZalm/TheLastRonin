#ifndef TRACK_H
#define TRACK_H

#include <QString>

struct Track {
    int id;
    QString title;
    QString filePath;
    QString artist;
    QString album;
    qint64 duration = 0;
    int trackNumber;
};

#endif // TRACK_H
