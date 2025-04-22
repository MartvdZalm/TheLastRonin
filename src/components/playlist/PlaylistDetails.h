#ifndef PLAYLISTDETAILS_H
#define PLAYLISTDETAILS_H

#include "../../model/Playlist.h"
#include <QWidget>
#include <QLabel>

class PlaylistDetails : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistDetails(const Playlist& playlist, QWidget* parent = nullptr);

private:
    QLabel* nameLabel;
    QLabel* descriptionLabel;
    QLabel* tracksLabel;
};


#endif // PLAYLISTDETAILS_H
