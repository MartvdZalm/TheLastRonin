#ifndef PLAYLISTGRID_H
#define PLAYLISTGRID_H

#include <QObject>
#include <QGridLayout>
#include "../../model/Playlist.h"

class PlaylistGrid : public QObject
{
    Q_OBJECT

public:
    explicit PlaylistGrid(QGridLayout* layout, QObject* parent = nullptr);

    void setLayout(QGridLayout* newLayout);
    void addPlaylist(const Playlist& playlist);
    void clearGrid();
    void openPlaylistPage(const Playlist& playlist);
    void updateColumns();

private:
    QGridLayout* layout;
    int currentRow = 0;
    int currentCol = 0;
    int maxCols = 4;
    const int cardWidth = 250;
    const int minSpacing = 15;

    void calculateMaxColumns();

signals:
    void playlistClicked(const Playlist& playlist);
};

#endif // PLAYLISTGRID_H
