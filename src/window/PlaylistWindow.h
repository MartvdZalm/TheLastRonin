#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include "../model/Playlist.h"
#include "../components/playlist/TrackList.h"
#include "../dao/TrackDAO.h"
#include "../dao/PlaylistDAO.h"
#include "BaseWindow.h"
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

    void setupUI() override;
    void setupConnections() override;
    void reload() override;
    void setStyle() override;

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
    TrackList* trackList;
    int currentTrackIndex = 0;

    TrackDAO trackDAO;
    PlaylistDAO playlistDAO;

    std::optional<Playlist> showEditPlaylistDialog();
    void updateTimeLabel(qint64 position, qint64 duration, QLabel* label);
    void playTrackAtIndex(int index);
    void playNextTrack();
    void loadPlaylist(const Playlist& playlist);
    void initPlayer();
    void createControlButtons();
    void styleButton(QPushButton* button, const QString& iconPath = "");
    QWidget* createPlayerBar();
};

#endif // PLAYLISTWINDOW_H
