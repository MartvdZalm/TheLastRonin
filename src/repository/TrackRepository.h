#ifndef TRACKREPOSITORY_H
#define TRACKREPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../model/Track.h"
#include "Repository.h"
#include <QObject>

class TrackRepository : public QObject
{
    Q_OBJECT

  public:
    explicit TrackRepository(QObject* parent = nullptr);

    bool save(Track* track);
    std::unique_ptr<Track> find(int id);
    QList<std::unique_ptr<Track>> findAll();

  private:
    std::shared_ptr<Repository<Track>> baseRepository;
};
#endif // TRACKREPOSITORY_H
