#ifndef ALBUM_H
#define ALBUM_H

#include <QString>

struct Album
{
public:
    int id;
    QString title;
    QString artist;
    QString coverPath;
    QString releaseDate;
};

#endif // ALBUM_H
