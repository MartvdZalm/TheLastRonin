#ifndef TRACKLIST_H
#define TRACKLIST_H

#include "../../model/Track.h"
#include <QListWidget>

class TrackList : public QListWidget
{
    Q_OBJECT

  public:
    explicit TrackList(const QVector<Track>& tracks, QWidget* parent = nullptr);
    void addTrack(const Track& track);
    QWidget* createTrackItemWidget(const Track& track, int index);

  private:
    bool eventFilter(QObject* obj, QEvent* event) override;
    QString formatDuration(qint64 milliseconds) const;
    void updateItemWidgetStyle(QWidget* widget, bool hovered);
};

#endif // TRACKLIST_H
