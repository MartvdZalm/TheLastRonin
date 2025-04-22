#include "PlaylistGrid.h"
#include "PlaylistCard.h"
#include "../../window/PlaylistWindow.h"

PlaylistGrid::PlaylistGrid(QGridLayout* layout, QObject* parent)
    : QObject(parent), layout(layout)
{
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void PlaylistGrid::setLayout(QGridLayout* newLayout)
{
    layout = newLayout;
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void PlaylistGrid::addPlaylist(const Playlist& playlist)
{
    PlaylistCard* card = new PlaylistCard(playlist);
    connect(card, &PlaylistCard::playlistClicked, this, &PlaylistGrid::openPlaylistPage);
    layout->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentCol >= maxCols) {
        currentCol = 0;
        currentRow++;
    }
}

void PlaylistGrid::clearGrid()
{
    if (!layout) {
        return;
    }

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    currentRow = 0;
    currentCol = 0;
}

void PlaylistGrid::openPlaylistPage(const Playlist& playlist)
{
    PlaylistWindow* page = new PlaylistWindow(playlist);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->show();
}

