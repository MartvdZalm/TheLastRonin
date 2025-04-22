#ifndef PLAYLISTCARD_H
#define PLAYLISTCARD_H

#include <QWidget>
#include <QLabel>
#include "../../model/Playlist.h"

class PlaylistCard : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistCard(const Playlist& playlist, QWidget* parent = nullptr);

signals:
    void playlistClicked(Playlist playlist);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Playlist playlistData;
    QLabel* previewLabel;
};

#endif // PLAYLISTCARD_H
