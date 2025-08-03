#include "PlaylistRepository.h"
#include "../database/DatabaseManager.h"
#include "../database/ORM.h"
#include "../database/QueryBuilder.h"

PlaylistRepository::PlaylistRepository(QObject* parent) : QObject(parent)
{
    baseRepository = ORM::instance().getRepository<Playlist>();
}

bool PlaylistRepository::save(Playlist* playlist)
{
    return baseRepository->save(playlist);
}

std::unique_ptr<Playlist> PlaylistRepository::find(int id)
{
    return baseRepository->find(id);
}

std::vector<std::unique_ptr<Playlist>> PlaylistRepository::findAll()
{
    return baseRepository->findAll();
}


