#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include "../model/Playlist.h"
#include "../components/playlist/TrackList.h"
#include "../dao/TrackDAO.h"
#include "../dao/PlaylistDAO.h"
#include "BaseWindow.h"
#include "../components/playlist/PlaylistDetails.h"
#include "../components/shared/CoverImageWidget.h"
#include "../components/playlist/MiniPlayerWindow.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <optional>

class PlaylistWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit PlaylistWindow(const Playlist& playlist, QWidget* parent = nullptr);
    ~PlaylistWindow();

    void setupUI() override;
    void setupConnections() override;
    void setStyle() override;
    void setupEvents() override;

private:
    Playlist playlistData;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QLabel* songLabel;
    QSlider* progressSlider;
    QLabel* timeLabel;
    QPushButton* addTrackBtn;
    QPushButton* editPlaylistBtn;
    QPushButton* removePlaylistBtn;
    QPushButton* pausePlayButton;
    QPushButton* prevButton;
    QPushButton* nextButton;
    PlaylistDetails* detailsWidget;
    CoverImageWidget* coverImageWidget;
    TrackList* trackList;
    int currentTrackIndex = 0;

    MiniPlayerWindow* miniPlayer;
    QPushButton* miniPlayerToggleButton;
    bool isMiniPlayerActive;

    QSlider* volumeSlider;
    QPushButton* muteButton;
    bool isMuted = false;
    int storedVolume = 50;

    TrackDAO trackDAO;
    PlaylistDAO playlistDAO;

    std::optional<Playlist> showEditPlaylistDialog();
    void updateTimeLabel(qint64 position, qint64 duration, QLabel* label);
    void playTrackAtIndex(int index);
    void playNextTrack();
    void refreshMetadata(const Playlist& updatedPlaylist);
    void initPlayer();
    void createControlButtons();
    void styleButton(QPushButton* button, const QString& iconPath = "");
    QWidget* createPlayerBar();

private slots:
    void toggleMiniPlayer();
    void onMiniPlayerClosed();
    void syncMiniPlayerControls();
};

#endif // PLAYLISTWINDOW_H
