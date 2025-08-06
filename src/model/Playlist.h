#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Model.h"
#include "Track.h"
#include <QDateTime>
#include <QString>

class Playlist : public Model
{
  public:
    Playlist() = default;

    QString getName() const
    {
        return name;
    }

    QString getDescription() const
    {
        return description;
    }

    QString getCoverImagePath() const
    {
        return coverImagePath;
    }

    QList<Track> getTracks() const
    {
        return tracks;
    }

    void addTrack(Track track)
    {
        tracks.append(track);
    }

    void setName(const QString& name)
    {
        this->name = name;
    }

    void setDescription(const QString& description)
    {
        this->description = description;
    }

    void setCoverImagePath(const QString& coverImagePath)
    {
        this->coverImagePath = coverImagePath;
    }

    void setTracks(const QList<Track>& tracks)
    {
        this->tracks = tracks;
    }

  private:
    QString name;
    QString description;
    QString coverImagePath;
    QList<Track> tracks;
};

#endif // PLAYLIST_H
