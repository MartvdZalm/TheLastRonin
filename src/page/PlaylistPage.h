#ifndef PLAYLISTPAGE_H
#define PLAYLISTPAGE_H

#include "../playlist/Playlist.h"
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

private:
    Playlist playlistData;
    QLabel* coverImageLabel;
    QLabel* nameLabel;
    QLabel* descriptionLabel;
    QListWidget* tracksListWidget;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
};

#endif // PLAYLISTPAGE_H
