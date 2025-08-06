#ifndef TRACKREPOSITORY_H
#define TRACKREPOSITORY_H

#include "ITrackRepository.h"
#include <QSqlDatabase>

class TrackRepository : public ITrackRepository
{
  public:
    explicit TrackRepository(QSqlDatabase& db);

    std::optional<Track> findById(int id) override;
    QList<Track> findAll() override;
    std::optional<Track> save(const Track& track) override;
    bool deleteById(int id) override;
    std::optional<Track> findByFilePath(const QString& filePath) override;

  private:
    QSqlDatabase& database;

    Track mapFromRecord(const QSqlQuery& query);
    std::optional<Track> insert(const Track& track);
    bool update(const Track& track);
};

#endif // TRACKREPOSITORY_H
