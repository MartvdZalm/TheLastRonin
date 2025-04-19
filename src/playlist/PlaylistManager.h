#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include "../database/DatabaseManager.h"
#include <QGridLayout>

class PlaylistManager : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistManager(QObject* parent = nullptr);

    static PlaylistManager& instance();

    void setPlaylistGrid(QGridLayout* playlistGrid);
    void openAddPlaylistDialog(QWidget* parent);
    void addPlaylistToGrid(const Playlist& playlist);
    void applyFilterToGrid(const QString& selectedFilter);
    void clearPlaylistGrid();

private slots:
    void openPlaylistPage(const Playlist& playlist);

private:
    DatabaseManager* db;
    QGridLayout* playlistGrid;

    int currentRow = 0;
    int currentCol = 0;
    int maxCols = 4;
};

#endif // PLAYLISTMANAGER_H
