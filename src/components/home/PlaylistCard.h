#ifndef PLAYLISTCARD_H
#define PLAYLISTCARD_H

#include "../../model/Playlist.h"
#include <QLabel>
#include <QWidget>

class PlaylistCard : public QWidget
{
    Q_OBJECT

  private:
    Playlist playlist;

  public:
    explicit PlaylistCard(const Playlist& playlist, QWidget* parent = nullptr);

  private:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // PLAYLISTCARD_H
