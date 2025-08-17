#ifndef TRACKLIST_H
#define TRACKLIST_H

#include "../../model/Track.h"
#include <QListWidget>
#include <QMenu>

class TrackList : public QListWidget
{
    Q_OBJECT

  public:
    explicit TrackList(const QList<Track>& tracks, QWidget* parent = nullptr, bool showMenu = true);
    void addTrack(const Track& track);
    void updateTracks(const QList<Track>& tracks);
    QWidget* createTrackItemWidget(const Track& track, int index);

  private:
    bool eventFilter(QObject* obj, QEvent* event) override;
    QString formatDuration(qint64 milliseconds) const;
    void updateItemWidgetStyle(QWidget* widget, bool hovered);

    bool showMenu;
    QMenu* currentMenu = nullptr;
};

#endif // TRACKLIST_H
