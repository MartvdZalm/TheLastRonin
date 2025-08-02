#ifndef PLAYLISTTRACK_H
#define PLAYLISTTRACK_H

#include "Model.h"

class PlaylistTrack : public Model
{
    Q_OBJECT
    Q_PROPERTY(int playlistId READ getPlaylistId WRITE setPlaylistId NOTIFY playlistIdChanged)
    Q_PROPERTY(int trackId READ getTrackId WRITE setTrackId NOTIFY trackIdChanged)
    Q_PROPERTY(int position READ getPosition WRITE setPosition NOTIFY positionChanged)

  public:
    explicit PlaylistTrack(QObject* parent = nullptr);

    int getPlaylistId() const
    {
        return playlistId;
    }

    int getTrackId() const
    {
        return trackId;
    }

    int getPosition() const
    {
        return position;
    }

    void setPlaylistId(int playlistId);
    void setTrackId(int trackId);
    void setPosition(int position);

    QString getTableName() const override
    {
        return "playlist_tracks";
    }

    QStringList tableSchema() const override;

  signals:
    void playlistIdChanged();
    void trackIdChanged();
    void positionChanged();

  private:
    int playlistId;
    int trackId;
    int position;
};

#endif // PLAYLISTTRACK_H
