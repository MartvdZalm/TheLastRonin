#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "../model/Model.h"
#include <QDateTime>
#include <QString>

class Playlist : public Model
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString coverImagePath READ getCoverImagePath WRITE setCoverImagePath NOTIFY coverImagePathChanged)
    Q_PROPERTY(QDateTime createdAt READ getCreatedAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ getUpdatedAt WRITE setUpdatedAt NOTIFY updatedAtChanged)

  public:
    explicit Playlist(QObject* parent = nullptr);

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

    QDateTime getCreatedAt() const
    {
        return createdAt;
    }

    QDateTime getUpdatedAt() const
    {
        return updatedAt;
    }

    void setName(const QString& name);
    void setDescription(const QString& description);
    void setCoverImagePath(const QString& coverImagePath);
    void setCreatedAt(const QDateTime& createdAt);
    void setUpdatedAt(const QDateTime& updatedAt);

    void deserialize(const QSqlRecord& record) override;

    QString getTableName() const override
    {
        return "playlists";
    }

    QStringList tableSchema() const override;

    bool hasTimestamps() const override
    {
        return true;
    }

    bool isValid() const override;
    QStringList validationErrors() const override;

  signals:
    void nameChanged();
    void descriptionChanged();
    void coverImagePathChanged();
    void createdAtChanged();
    void updatedAtChanged();

  private:
    QString name;
    QString description;
    QString coverImagePath;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // PLAYLIST_H