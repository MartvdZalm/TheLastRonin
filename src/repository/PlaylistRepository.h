#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H

#include "../database/QueryBuilder.h"
#include "../repository/Repository.h"
#include "../model/Playlist.h"
#include "../model/Track.h"
#include <QObject>
#include <memory>

class PlaylistRepository : public QObject
{
    Q_OBJECT

  public:
    explicit PlaylistRepository(QObject* parent = nullptr);

    bool save(Playlist* playlist);
    std::unique_ptr<Playlist> find(int id);
    QList<std::unique_ptr<Playlist>> findAll();

  private:
    std::shared_ptr<Repository<Playlist>> baseRepository;
    std::shared_ptr<Repository<Track>> trackRepository;
};

#endif // PLAYLISTREPOSITORY_H