#include "PlaylistGrid.h"
#include "../../window/PlaylistWindow.h"
#include "PlaylistCard.h"

PlaylistGrid::PlaylistGrid(QGridLayout* layout, QObject* parent) : QObject(parent), layout(layout)
{
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    calculateMaxColumns();
}

void PlaylistGrid::calculateMaxColumns()
{
    if (!layout || !layout->parentWidget())
    {
        maxCols = 4;
        return;
    }

    int availableWidth = layout->parentWidget()->width();

    QMargins margins = layout->contentsMargins();
    availableWidth -= (margins.left() + margins.right());

    maxCols = qMax(1, availableWidth / (cardWidth + minSpacing));
}

void PlaylistGrid::updateColumns()
{
    int savedRow = currentRow;
    int savedCol = currentCol;

    calculateMaxColumns();

    currentRow = 0;
    currentCol = 0;

    QList<QLayoutItem*> items;
    while (QLayoutItem* item = layout->takeAt(0))
    {
        items.append(item);
    }

    for (QLayoutItem* item : items)
    {
        layout->addWidget(item->widget(), currentRow, currentCol);
        currentCol++;
        if (currentCol >= maxCols)
        {
            currentCol = 0;
            currentRow++;
        }
    }
}

void PlaylistGrid::addPlaylist(const Playlist& playlist)
{
    PlaylistCard* card = new PlaylistCard(playlist);
    layout->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentCol >= maxCols)
    {
        currentCol = 0;
        currentRow++;
    }
}

void PlaylistGrid::clearGrid()
{
    if (!layout)
    {
        return;
    }

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    currentRow = 0;
    currentCol = 0;
}
