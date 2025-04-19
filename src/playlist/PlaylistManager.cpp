#include "PlaylistManager.h"

#include "../dialog/AddPlaylistDialog.h"
#include "../page/PlaylistPage.h"

PlaylistManager::PlaylistManager()
{
    playlistRepository = new PlaylistRepository(DatabaseManager::instance());
}

PlaylistManager& PlaylistManager::instance()
{
    static PlaylistManager instance;
    return instance;
}

void PlaylistManager::setPlaylistGrid(QGridLayout* layout, QWidget* parentForDialogs)
{
    gridController = new PlaylistGridController(layout);
    dialogParent = parentForDialogs;
    gridController->setLayout(layout);
}

void PlaylistManager::refreshGrid()
{
    gridController->clearGrid();

    QVector<Playlist> playlists = playlistRepository->getAll();
    for (const Playlist& playlist : playlists) {
        gridController->addPlaylist(playlist);
    }
}

void PlaylistManager::search(const QString& query)
{
    QList<Playlist> results = playlistRepository->search(query);
    gridController->clearGrid();

    for (const Playlist& playlist : results) {
        gridController->addPlaylist(playlist);
    }
}

void PlaylistManager::openAddPlaylistDialog()
{
    AddPlaylistDialog dialog(dialogParent);
    if (dialog.exec() == QDialog::Accepted) {
        Playlist playlist {
            .name = dialog.getName(),
            .description = dialog.getDescription(),
            .coverImagePath = dialog.getCoverImagePath(),
        };

        playlist.id = playlistRepository->insert(playlist);
        gridController->addPlaylist(playlist);
    }
}

void PlaylistManager::applyFilterToGrid(const QString& selectedFilter)
{
    // Optional if needed later
}

void PlaylistManager::addTrackToPlaylist(int playlistId, const Track& track)
{
    playlistRepository->addTrackToPlaylist(playlistId, track);
}

QVector<Track> PlaylistManager::getTracksForPlaylist(int playlistId)
{
    return playlistRepository->getTracksForPlaylist(playlistId);
}

