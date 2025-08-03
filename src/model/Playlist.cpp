#include "Playlist.h"

Playlist::Playlist(QObject* parent) : Model(parent)
{
    createdAt = QDateTime::currentDateTime();
    updatedAt = createdAt;
}

void Playlist::setName(const QString& name)
{
    if (this->name != name)
    {
        this->name = name;
        emit nameChanged();
        emitModelChanged();
    }
}

void Playlist::setDescription(const QString& description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
        emitModelChanged();
    }
}

void Playlist::setCoverImagePath(const QString& coverImagePath)
{
    if (this->coverImagePath != coverImagePath)
    {
        this->coverImagePath = coverImagePath;
        emit coverImagePathChanged();
        emitModelChanged();
    }
}

void Playlist::setCreatedAt(const QDateTime& createdAt)
{
    if (this->createdAt != createdAt)
    {
        this->createdAt = createdAt;
        emit createdAtChanged();
        emitModelChanged();
    }
}

void Playlist::setUpdatedAt(const QDateTime& updatedAt)
{
    if (this->updatedAt != updatedAt)
    {
        this->updatedAt = updatedAt;
        emit updatedAtChanged();
        emitModelChanged();
    }
}

void Playlist::deserialize(const QSqlRecord& record)
{
    setId(record.value("id").toInt());
    setName(record.value("name").toString());
    setDescription(record.value("description").toString());
    setCoverImagePath(record.value("coverImagePath").toString());
    setCreatedAt(record.value("createdAt").toDateTime());
    setUpdatedAt(record.value("updatedAt").toDateTime());
}

void Playlist::fromVariantMap(const QVariantMap& map)
{
    name = map["name"].toString();
    description = map["description"].toString();
    coverImagePath = map["coverImagePath"].toString();
}

QVariantMap Playlist::toVariantMap() const
{
    QVariantMap map;
    map["name"] = name;
    map["description"] = description;
    map["coverImagePath"] = coverImagePath;

    qDebug() << "Playlist toVariantMap():" << map;
    return map;
}

QStringList Playlist::tableSchema() const
{
    return {
        "id INTEGER PRIMARY KEY AUTOINCREMENT",
        "name TEXT NOT NULL",
        "description TEXT",
        "coverImagePath TEXT",
        "createdAt DATETIME DEFAULT CURRENT_TIMESTAMP",
        "updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP"
    };
}

bool Playlist::isValid() const
{
    return !name.isEmpty();
}

QStringList Playlist::validationErrors() const
{
    QStringList errors;
    if (name.isEmpty())
    {
        errors << "Playlist name is required";
    }
    return errors;
}
