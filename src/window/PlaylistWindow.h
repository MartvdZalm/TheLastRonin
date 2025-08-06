#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include "../components/playlist/PlaylistDetails.h"
#include "../components/playlist/TrackList.h"
#include "../components/shared/CoverImageWidget.h"
#include "../components/shared/MiniPlayerWindow.h"
#include "../components/shared/PlaybackBar.h"
#include "../model/Playlist.h"
#include "../repository/PlaylistRepository.h"
#include "../repository/TrackRepository.h"

#include "BaseWindow.h"
#include <QAudioOutput>
#include <QLabel>
#include <QListWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <optional>


class PlaylistWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit PlaylistWindow(const Playlist& playlist, QWidget* parent = nullptr);
    ~PlaylistWindow();

    void setupUI() override;
    void setupConnections() override;
    void setupEvents() override;

  private:
    Playlist playlist;
    QPushButton* addTrackBtn;
    QPushButton* editPlaylistBtn;
    QPushButton* removePlaylistBtn;
    PlaylistDetails* detailsWidget;
    CoverImageWidget* coverImageWidget;
    PlaybackBar* playbackBarWidget;
    TrackList* trackList;
    int currentTrackIndex = 0;

    std::optional<Playlist> showEditPlaylistDialog();
    void playTrackAtIndex(int index);
    void refreshMetadata(const Playlist& updatedPlaylist);
    void styleButton(QPushButton* button, const QString& iconPath = "");
};

#endif // PLAYLISTWINDOW_H
