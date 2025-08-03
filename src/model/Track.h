#ifndef TRACK_H
#define TRACK_H

#include "Model.h"
#include <QString>
#include "../database/ORM.h"

class Track : public Model
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString artist READ getArtist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ getAlbum WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(QString duration READ getDuration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(int trackNumber READ getTrackNumber WRITE setTrackNumber NOTIFY trackNumberChanged)

  public:
    explicit Track(QObject* parent = nullptr);

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

    int getTrackNumber() const
    {
        return trackNumber;
    }

    void setTitle(const QString& title);
    void setFilePath(const QString& filePath);
    void setArtist(const QString& artist);
    void setAlbum(const QString& album);
    void setDuration(const QString& duration);
    void setTrackNumber(int trackNumber);

    void deserialize(const QSqlRecord& record) override;
    void fromVariantMap(const QVariantMap& map) override;
    QVariantMap toVariantMap() const override;

    QString getTableName() const override
    {
        return "tracks";
    }

    QStringList tableSchema() const override;

    bool isValid() const override;
    QStringList validationErrors() const override;

  signals:
    void titleChanged();
    void filePathChanged();
    void artistChanged();
    void albumChanged();
    void durationChanged();
    void trackNumberChanged();

  private:
    QString title;
    QString filePath;
    QString artist;
    QString album;
    QString duration;
    int trackNumber;
};
MODEL_REGISTRATION(Track);

#endif // TRACK_H
