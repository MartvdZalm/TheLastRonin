#include "AlbumManager.h"

#include "../dialog/AddAlbumDialog.h"
#include "../database/DatabaseManager.h"
#include "AlbumCard.h"

AlbumManager::AlbumManager()
    : db(&DatabaseManager::instance())
{
    if (db->openDatabase()) {
        db->initSchema();
    }
}

AlbumManager& AlbumManager::instance()
{
    static AlbumManager instance;
    return instance;
}

void AlbumManager::setAlbumGrid(QGridLayout* albumGrid)
{
    this->albumGrid = albumGrid;
    albumGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void AlbumManager::openAddAlbumDialog(QWidget* parent)
{
    AddAlbumDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        Album newAlbum {
            .title = dialog.getTitle(),
            .artist = dialog.getArtist(),
            .coverPath = dialog.getCoverPath(),
            .releaseDate = dialog.getReleasDate()
        };

        int id = db->insertAlbum(newAlbum.title, newAlbum.artist, newAlbum.coverPath, newAlbum.releaseDate);
        newAlbum.id = id;

        addAlbumToGrid(newAlbum);
    }
}

void AlbumManager::addAlbumToGrid(const Album& album)
{
    if (!albumGrid) return;

    AlbumCard* card = new AlbumCard(album);
    albumGrid->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentCol >= maxCols) {
        currentCol = 0;
        currentRow++;
    }
}

void AlbumManager::applyFilterToGrid(const QString& selectedFilter)
{
    QVector<Album> filteredAlbums;

    if (selectedFilter == "All") {
        filteredAlbums = db->getAllAlbums();
    }

    clearAlbumGrid();
    for (const Album& album : filteredAlbums) {
        addAlbumToGrid(album);
    }
}

void AlbumManager::clearAlbumGrid()
{
    if (!albumGrid) return;

    QLayoutItem* item;
    while ((item = albumGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    currentRow = 0;
    currentCol = 0;
}
