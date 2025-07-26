#ifndef PLAYLISTDETAILS_H
#define PLAYLISTDETAILS_H

#include "../../model/Playlist.h"
#include <QLabel>
#include <QWidget>

class PlaylistDetails : public QWidget
{
    Q_OBJECT
  public:
    explicit PlaylistDetails(const Playlist& playlist, QWidget* parent = nullptr);

    void updateDetails(const Playlist& playlist);

  private:
    QLabel* nameLabel;
    QLabel* descriptionLabel;
    QLabel* tracksLabel;
};

#endif // PLAYLISTDETAILS_H
