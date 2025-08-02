#include "TrackRepository.h"

#include "../database/ORM.h"

TrackRepository::TrackRepository(QObject* parent) : QObject(parent)
{
    baseRepository = ORM::instance().getRepository<Track>();
}

bool TrackRepository::save(Track* track)
{
    return baseRepository->save(track);
}

std::unique_ptr<Track> TrackRepository::find(int id)
{
    return baseRepository->find(id);
}

QList<std::unique_ptr<Track>> TrackRepository::findAll()
{
    return baseRepository->findAll();
}