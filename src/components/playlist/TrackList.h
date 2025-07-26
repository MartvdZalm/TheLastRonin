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

    void setupStyle();
    QWidget* createTrackItemWidget(const Track& track, int index);

  protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

  private:
    QString formatDuration(qint64 milliseconds) const;
    void updateItemWidgetStyle(QWidget* widget, bool hovered);
};

#endif // TRACKLIST_H
