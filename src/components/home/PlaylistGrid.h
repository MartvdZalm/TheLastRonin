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

private:
    QGridLayout* layout;
    int currentRow = 0;
    int currentCol = 0;
    const int maxCols = 4;

signals:
    void playlistClicked(const Playlist& playlist);
};

#endif // PLAYLISTGRID_H
