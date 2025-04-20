#ifndef TRACKLISTWIDGET_H
#define TRACKLISTWIDGET_H

#include "Track.h"
#include <QListWidget>

class TrackListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit TrackListWidget(const QVector<Track>& tracks, QWidget* parent = nullptr);
    void addTrack(const Track& track);

    void setupStyle();
    QWidget* createTrackItemWidget(const Track& track, int index);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QString formatDuration(qint64 milliseconds) const;
    void updateItemWidgetStyle(QWidget* widget, bool hovered);

};

#endif // TRACKLISTWIDGET_H
