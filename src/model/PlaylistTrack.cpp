#include "PlaylistTrack.h"

PlaylistTrack::PlaylistTrack(QObject* parent) : Model(parent), playlistId(0), trackId(0), position(0) {}

void PlaylistTrack::setPlaylistId(int playlistId)
{
    if (this->playlistId != playlistId)
    {
        this->playlistId = playlistId;
        emit playlistIdChanged();
        emitModelChanged();
    }
}

void PlaylistTrack::setTrackId(int trackId)
{
    if (this->trackId != trackId)
    {
        this->trackId = trackId;
        emit trackIdChanged();
        emitModelChanged();
    }
}

void PlaylistTrack::setPosition(int position)
{
    if (this->position != position)
    {
        this->position = position;
        emit positionChanged();
        emitModelChanged();
    }
}

QStringList PlaylistTrack::tableSchema() const
{
    return {
        "id INTEGER PRIMARY KEY AUTOINCREMENT",
        "playlist_id INTEGER NOT NULL",
        "track_id INTEGER NOT NULL",
        "position INTEGER DEFAULT 0",
        "FOREIGN KEY (playlist_id) REFERENCES playlists(id) ON DELETE CASCADE",
        "FOREIGN KEY (track_id) REFERENCES tracks(id) ON DELETE CASCADE",
        "UNIQUE(playlist_id, track_id)"
    };
}
