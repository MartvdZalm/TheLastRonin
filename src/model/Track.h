#ifndef TRACK_H
#define TRACK_H

#include "Model.h"
#include <QString>

class Track : public Model
{
  public:
    Track() = default;

    QString getTitle() const
    {
        return title;
    }

    QString getFilePath() const
    {
        return filePath;
    }

    QString getArtist() const
    {
        return artist;
    }

    QString getAlbum() const
    {
        return album;
    }

    QString getDuration() const
    {
        return duration;
    }

    void setTitle(const QString& title)
    {
        this->title = title;
    }

    void setFilePath(const QString& filePath)
    {
        this->filePath = filePath;
    }

    void setArtist(const QString& artist)
    {
        this->artist = artist;
    }

    void setAlbum(const QString& album)
    {
        this->album = album;
    }

    void setDuration(const QString& duration)
    {
        this->duration = duration;
    }

  private:
    QString title;
    QString filePath;
    QString artist;
    QString album;
    QString duration;
};

#endif // TRACK_H
