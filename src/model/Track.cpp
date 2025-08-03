#include "Track.h"

Track::Track(QObject* parent) : Model(parent), trackNumber(0) {}

void Track::setTitle(const QString& title)
{
    if (this->title != title)
    {
        this->title = title;
        emit titleChanged();
        emitModelChanged();
    }
}

void Track::setFilePath(const QString& filePath)
{
    if (this->filePath != filePath)
    {
        this->filePath = filePath;
        emit filePathChanged();
        emitModelChanged();
    }
}

void Track::setArtist(const QString& artist)
{
    if (this->artist != artist)
    {
        this->artist = artist;
        emit artistChanged();
        emitModelChanged();
    }
}

void Track::setAlbum(const QString& album)
{
    if (this->album != album)
    {
        this->album = album;
        emit albumChanged();
        emitModelChanged();
    }
}

void Track::setDuration(const QString& duration)
{
    if (this->duration != duration)
    {
        this->duration = duration;
        emit durationChanged();
        emitModelChanged();
    }
}

void Track::setTrackNumber(int trackNumber)
{
    if (this->trackNumber != trackNumber)
    {
        this->trackNumber = trackNumber;
        emit trackNumberChanged();
        emitModelChanged();
    }
}

void Track::deserialize(const QSqlRecord& record)
{
    setId(record.value("id").toInt());
    setTitle(record.value("title").toString());
    setFilePath(record.value("filePath").toString());
    setArtist(record.value("artist").toString());
    setAlbum(record.value("album").toString());
    setDuration(record.value("duration").toString());
    setTrackNumber(record.value("trackNumber").toInt());
}

void Track::fromVariantMap(const QVariantMap& map)
{
    title = map["title"].toString();
    filePath = map["filePath"].toString();
    artist = map["artist"].toString();
    album = map["album"].toString();
    duration = map["duration"].toString();
    trackNumber = map["trackNumber"].toInt();
}

QVariantMap Track::toVariantMap() const
{
    QVariantMap map;
    map["title"] = title;
    map["filePath"] = filePath;
    map["artist"] = artist;
    map["album"] = album;
    map["duration"] = duration;
    map["trackNumber"] = trackNumber;
    return map;
}

QStringList Track::tableSchema() const
{
    return {"id INTEGER PRIMARY KEY AUTOINCREMENT",
            "title TEXT NOT NULL",
            "filePath TEXT UNIQUE NOT NULL",
            "artist TEXT",
            "album TEXT",
            "duration TEXT",
            "trackNumber INTEGER DEFAULT 0"};
}

bool Track::isValid() const
{
    return !title.isEmpty() && !filePath.isEmpty();
}

QStringList Track::validationErrors() const
{
    QStringList errors;
    if (title.isEmpty())
    {
        errors << "Title is required";
    }
    if (filePath.isEmpty())
    {
        errors << "File path is required";
    }
    return errors;
}
