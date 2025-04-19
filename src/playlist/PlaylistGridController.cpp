#include "PlaylistGridController.h"
#include "PlaylistCard.h"

#include "../page/PlaylistPage.h"

PlaylistGridController::PlaylistGridController(QGridLayout* layout, QObject* parent)
    : QObject(parent), layout(layout)
{
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void PlaylistGridController::setLayout(QGridLayout* newLayout)
{
    layout = newLayout;
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void PlaylistGridController::addPlaylist(const Playlist& playlist)
{
    PlaylistCard* card = new PlaylistCard(playlist);
    connect(card, &PlaylistCard::playlistClicked, this, &PlaylistGridController::openPlaylistPage);
    layout->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentCol >= maxCols) {
        currentCol = 0;
        currentRow++;
    }
}

void PlaylistGridController::clearGrid()
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

void PlaylistGridController::openPlaylistPage(const Playlist& playlist)
{
    PlaylistPage* page = new PlaylistPage(playlist);
    page->setAttribute(Qt::WA_DeleteOnClose);
    page->show();
}

