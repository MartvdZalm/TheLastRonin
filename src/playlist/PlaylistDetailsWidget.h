#ifndef PLAYLISTDETAILSWIDGET_H
#define PLAYLISTDETAILSWIDGET_H

#include "Playlist.h"
#include <QWidget>
#include <QLabel>

class PlaylistDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistDetailsWidget(const Playlist& playlist, QWidget* parent = nullptr);

private:
    QLabel* nameLabel;
    QLabel* descriptionLabel;
};


#endif // PLAYLISTDETAILSWIDGET_H
