#ifndef PLAYLISTGRID_H
#define PLAYLISTGRID_H

#include "../../model/Playlist.h"
#include <QGridLayout>
#include <QObject>

class PlaylistGrid : public QObject
{
    Q_OBJECT

  public:
    explicit PlaylistGrid(QGridLayout* layout, QObject* parent = nullptr);

    void setLayout(QGridLayout* newLayout);
    void addPlaylist(const Playlist& playlist);
    void clearGrid();
    void updateColumns();

  private:
    QGridLayout* layout;
    int currentRow = 0;
    int currentCol = 0;
    int maxCols = 4;
    const int cardWidth = 250;
    const int minSpacing = 15;

    void calculateMaxColumns();
};

#endif // PLAYLISTGRID_H
