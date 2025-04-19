#include "AlbumRepository.h"

QVector<Album> AlbumRepository::getAllAlbums()
{
    return db.getAllAlbums();
}

int AlbumRepository::insertAlbum(const Album& album)
{
    return db.insertAlbum(
        album.title,
        album.artist,
        album.coverPath,
        album.releaseDate
    );
}
