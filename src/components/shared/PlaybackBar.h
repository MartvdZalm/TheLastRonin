#ifndef PLAYBACKBAR_H
#define PLAYBACKBAR_H

#include "../../model/Playlist.h"
#include "../../model/Track.h"
#include "../shared/MiniPlayerWindow.h"
#include <QAudioOutput>
#include <QLabel>
#include <QListWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <optional>

class PlaybackBar : public QWidget
{
    Q_OBJECT

  private:
    const Playlist& playlist;
    const Track& currentTrack;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QLabel* songLabel;
    QSlider* progressSlider;
    QLabel* timeLabel;
    QPushButton* pausePlayButton;
    QPushButton* prevButton;
    QPushButton* nextButton;

    MiniPlayerWindow* miniPlayer;
    QPushButton* miniPlayerToggleButton;
    bool isMiniPlayerActive;

    QSlider* volumeSlider;
    QPushButton* muteButton;
    bool isMuted = false;
    int storedVolume = 50;
    int currentTrackIndex = 0;

  private:
    explicit PlaybackBar(QWidget* parent = nullptr);
    ~PlaybackBar();

  public:
    static PlaybackBar* instance();
    void setupUI();
    void setupConnections();
    void setStyle();
    void updatePlaylist(const Playlist& playlist);
    void playTrackAtIndex(int index);

  signals:
    void nextClicked();
    void prevClicked();

  private:
    void updateTimeLabel(qint64 position, qint64 duration, QLabel* label);
    void playNextTrack();
    QWidget* createPlayerBar();

  private slots:
    void toggleMiniPlayer();
    void onMiniPlayerClosed();
    void syncMiniPlayerControls();
    void onNextClicked();
    void onPrevClicked();
};

#endif // PLAYBACKBAR_H
