#ifndef PLAYLISTGRIDCONTROLLER_H
#define PLAYLISTGRIDCONTROLLER_H

#include <QObject>
#include <QGridLayout>
#include "Playlist.h"

class PlaylistGridController : public QObject
{
    Q_OBJECT

public:
    explicit PlaylistGridController(QGridLayout* layout, QObject* parent = nullptr);

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

#endif // PLAYLISTGRIDCONTROLLER_H
