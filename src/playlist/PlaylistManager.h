#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include "../repository/PlaylistRepository.h"
#include "PlaylistGridController.h"
#include <QGridLayout>
#include <optional>

class PlaylistManager
{
public:
    PlaylistManager();

    static PlaylistManager& instance();

    void setPlaylistGrid(QGridLayout* layout, QWidget* parentForDialogs);
    void openAddPlaylistDialog();
    std::optional<Playlist> openEditPlaylistDialog(const Playlist& playlist);
    void removePlaylist(int playlistId);
    void applyFilterToGrid(const QString& selectedFilter);
    void refreshGrid();
    void search(const QString& query);
    void addTrackToPlaylist(int playlistId, const Track& track);
    QVector<Track> getTracksForPlaylist(int playlistId);

private:
    PlaylistRepository* playlistRepository;
    PlaylistGridController* gridController = nullptr;
    QWidget* dialogParent = nullptr;
};

#endif // PLAYLISTMANAGER_H
