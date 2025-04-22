#ifndef DATABASECONSTANTS_H
#define DATABASECONSTANTS_H

#include <QString>

namespace Tables
{
    const QString Playlist = "playlist";
    const QString Track = "track";
}

namespace Columns
{
    const QString PlaylistId = "id";
    const QString PlaylistName = "name";
    const QString PlaylistDescription = "description";
    const QString PlaylistCoverImagePath = "coverImagePath";
    const QString PlaylistCreatedAt = "createdAt";
    const QString PlaylistUpdatedAt = "updatedAt";

    const QString TrackId = "id";
    const QString TrackTitle = "title";
    const QString TrackFilePath = "filePath";
    const QString TrackArtist = "artist";
    const QString TrackAlbum = "album";
    const QString TrackDuration = "duration";
    const QString TrackTrackNumber = "trackNumber";
    const QString TrackPlaylistId = "playlistId";
}

#endif // DATABASECONSTANTS_H
