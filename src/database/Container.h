#ifndef CONTAINER_H
#define CONTAINER_H

#include "../repository/ISettingRepository.h"
#include "../repository/IPlaylistRepository.h"
#include "../repository/ITrackRepository.h"
#include <QSqlDatabase>
#include <memory>

class Container
{
  public:
    static Container& instance();

    void initialize(QSqlDatabase& database);

    std::shared_ptr<ISettingRepository> getSettingRepository();
    std::shared_ptr<IPlaylistRepository> getPlaylistRepository();
    std::shared_ptr<ITrackRepository> getTrackRepository();

  private:
    Container() = default;

    QSqlDatabase* database = nullptr;
    std::shared_ptr<ISettingRepository> settingRepository;
    std::shared_ptr<IPlaylistRepository> playlistRepository;
    std::shared_ptr<ITrackRepository> trackRepository;
};

#endif // CONTAINER_H
