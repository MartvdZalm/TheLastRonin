#ifndef PLAYLISTPAGE_H
#define PLAYLISTPAGE_H

#include "../playlist/Playlist.h"
#include "../playlist/TrackListWidget.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

class PlaylistPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistPage(const Playlist& playlist, QWidget* parent = nullptr);
    void updateTimeLabel(qint64 position, qint64 duration, QLabel* label);
    void loadPlaylist(const Playlist& playlist);

private slots:
    void playNextTrack();
    void playTrackAtIndex(int index);

private:
    void setupUI();
    void setStyle();

    Playlist playlistData;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    TrackListWidget* trackListWidget;
    QLabel* songLabel;

    int currentTrackIndex = -1;
};

#endif // PLAYLISTPAGE_H
