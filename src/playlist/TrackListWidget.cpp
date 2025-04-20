#include "TrackListWidget.h"

#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>

TrackListWidget::TrackListWidget(const QVector<Track>& tracks, QWidget* parent)
    : QListWidget(parent)
{
    setupStyle();

    for (int i = 0; i < tracks.size(); ++i) {
        const Track& track = tracks[i];
        QListWidgetItem* item = new QListWidgetItem(this);
        item->setSizeHint(QSize(0, 50));
        item->setData(Qt::UserRole, QVariant::fromValue(track));

        setItemWidget(item, createTrackItemWidget(track, i + 1));
    }

}

void TrackListWidget::addTrack(const Track& track)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setSizeHint(QSize(0, 50));
    item->setData(Qt::UserRole, QVariant::fromValue(track));

    int index = count();
    setItemWidget(item, createTrackItemWidget(track, index + 1));
}

QWidget* TrackListWidget::createTrackItemWidget(const Track& track, int index)
{
    QWidget* itemWidget = new QWidget(this);
    itemWidget->setProperty("isHovered", false);

    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(15);

    QLabel* numberLabel = new QLabel(QString::number(index), itemWidget);
    numberLabel->setStyleSheet(R"(
        QLabel {
            color: #888;
            font-size: 12px;
            min-width: 25px;
        }
    )");

    QLabel* titleLabel = new QLabel(itemWidget);
    titleLabel->setStyleSheet(R"(
        QLabel {
            color: #e0e0e0;
            font-size: 14px;
        }
    )");

    QString titleText = track.title;
    if (!track.artist.isEmpty()) {
        titleText += "\n<span style='color: #aaa; font-size: 12px;'>" + track.artist + "</span>";
    }
    titleLabel->setText(titleText);
    titleLabel->setWordWrap(true);

    QLabel* durationLabel = new QLabel(formatDuration(track.duration.toInt()), itemWidget);
    durationLabel->setStyleSheet(R"(
        QLabel {
            color: #888;
            font-size: 12px;
            min-width: 40px;
            text-align: right;
        }
    )");

    layout->addWidget(numberLabel);
    layout->addWidget(titleLabel, 1);
    layout->addWidget(durationLabel);

    updateItemWidgetStyle(itemWidget, false);
    itemWidget->installEventFilter(this);
    itemWidget->setAttribute(Qt::WA_Hover);

    itemWidget->setLayout(layout);

    return itemWidget;
}

void TrackListWidget::updateItemWidgetStyle(QWidget* widget, bool hovered)
{
    widget->setProperty("isHovered", hovered);
    widget->setStyleSheet(QString(R"(
        QWidget {
            background-color: %1;
            border-radius: 4px;
        }
    )").arg(hovered ? "#383838" : "transparent"));
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}

bool TrackListWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::Enter) {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget) {
            updateItemWidgetStyle(widget, true);
        }
    } else if (event->type() == QEvent::Leave) {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget) {
            updateItemWidgetStyle(widget, false);
        }
    }
    return QListWidget::eventFilter(obj, event);
}

QString TrackListWidget::formatDuration(qint64 milliseconds) const
{
    if (milliseconds <= 0) return "--:--";

    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

void TrackListWidget::setupStyle()
{
    setStyleSheet(R"(
        TrackListWidget {
            background-color: #2d2d2d;
            border: 1px solid #3d3d3d;
            border-radius: 6px;
            padding: 2px;
            outline: none;
            font-family: 'Segoe UI', Roboto, sans-serif;
        }

        TrackListWidget::item {
            border-bottom: 1px solid #383838;
            background-color: transparent;
        }

        TrackListWidget::item:selected {
            background-color: #4a90e2;
            color: white;
            border-radius: 4px;
        }

        QScrollBar:vertical {
            border: none;
            background: #2d2d2d;
            width: 8px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #5a5a5a;
            min-height: 20px;
            border-radius: 4px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");

    setFrameShape(QFrame::NoFrame);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
