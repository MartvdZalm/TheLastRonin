#ifndef ALBUMREPOSITORY_H
#define ALBUMREPOSITORY_H

#include "../album/Album.h"
#include "../database/DatabaseManager.h"

class AlbumRepository
{
public:
    AlbumRepository(DatabaseManager& db) :  db(db) {}

    QVector<Album> getAllAlbums();
    int insertAlbum(const Album& album);

private:
    DatabaseManager& db;
};

#endif // ALBUMREPOSITORY_H
