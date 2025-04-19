#ifndef PLAYLISTPAGE_H
#define PLAYLISTPAGE_H

#include "../playlist/Playlist.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>

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
};

#endif // PLAYLISTPAGE_H
