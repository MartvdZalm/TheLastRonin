#ifndef PLAYLISTCARD_H
#define PLAYLISTCARD_H

#include "../../model/Playlist.h"
#include <QLabel>
#include <QWidget>

class PlaylistCard : public QWidget
{
    Q_OBJECT

  public:
    explicit PlaylistCard(const Playlist& playlist, QWidget* parent = nullptr);

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    Playlist playlistData;
    QLabel* previewLabel;
};

#endif // PLAYLISTCARD_H
