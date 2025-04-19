#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include "Track.h"
#include <QListWidget >

class TrackListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit TrackListWidget(const QVector<Track>& tracks, QWidget* parent = nullptr);
    void addTrack(const Track& track);
};

#endif // TRACKLISTWIDGET_H
