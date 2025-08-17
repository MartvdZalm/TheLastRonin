#ifndef ITRACKREPOSITORY_H
#define ITRACKREPOSITORY_H

#include "../model/Track.h"
#include <QSqlQuery>
#include <QString>
#include <optional>
#include <vector>

class ITrackRepository
{
  public:
    virtual ~ITrackRepository() = default;

    virtual std::optional<Track> findById(int id) = 0;
    virtual QList<Track> findAll() = 0;
    virtual std::optional<Track> save(const Track& track) = 0;
    virtual bool deleteById(int id) = 0;
    virtual std::optional<Track> findByFilePath(const QString& filePath) = 0;
    virtual Track mapFromRecord(const QSqlQuery& query) = 0;
};

#endif // ITRACKREPOSITORY_H
