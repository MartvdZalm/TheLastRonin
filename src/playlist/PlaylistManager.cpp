#include "PlaylistManager.h"

#include "../dialog/AddPlaylistDialog.h"
#include "../database/DatabaseManager.h"
#include "../page/PlaylistPage.h"
#include "PlaylistCard.h"

PlaylistManager::PlaylistManager(QObject* parent)
    : QObject(parent)
    , db(&DatabaseManager::instance())
{
    if (db->openDatabase()) {
        db->initSchema();
    }
}

PlaylistManager& PlaylistManager::instance()
{
    static PlaylistManager instance;
    return instance;
}

void PlaylistManager::setPlaylistGrid(QGridLayout* playlistGrid)
{
    this->playlistGrid = playlistGrid;
    playlistGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void PlaylistManager::openAddPlaylistDialog(QWidget* parent)
{
    AddPlaylistDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        Playlist newPlaylist {
            .name = dialog.getName(),
            .description = dialog.getDescription(),
            .coverImagePath = dialog.getCoverImagePath(),
        };

        int id = db->insertPlaylist(newPlaylist.name, newPlaylist.description, newPlaylist.coverImagePath);
        newPlaylist.id = id;

        addPlaylistToGrid(newPlaylist);
    }
}

void PlaylistManager::addPlaylistToGrid(const Playlist& playlist)
{
    if (!playlistGrid) {
        return;
    }

    PlaylistCard* card = new PlaylistCard(playlist);
    connect(card, &PlaylistCard::playlistClicked, this, &PlaylistManager::openPlaylistPage);
    playlistGrid->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentRow >= maxCols) {
        currentCol = 0;
        currentRow++;
    }
}

void PlaylistManager::applyFilterToGrid(const QString& selectedFilter)
{

}

void PlaylistManager::clearPlaylistGrid()
{
    if (!playlistGrid) {
        return;
    }

    QLayoutItem* item;
    while ((item = playlistGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    currentRow = 0;
    currentCol = 0;
}

void PlaylistManager::openPlaylistPage(const Playlist& playlist)
{
    PlaylistPage* page = new PlaylistPage(playlist);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->show();
}
