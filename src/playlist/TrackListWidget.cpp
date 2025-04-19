#include "TrackListWidget.h"

#include <QListWidget>

TrackListWidget::TrackListWidget(const QVector<Track>& tracks, QWidget* parent)
    : QListWidget(parent)
{
    for (const Track& track : tracks) {
        addItem(track.title + " - " + track.artist);
    }
}

void TrackListWidget::addTrack(const Track& track)
{
    QListWidgetItem* item = new QListWidgetItem(track.title + " - " + track.artist);
    this->addItem(item);
}
