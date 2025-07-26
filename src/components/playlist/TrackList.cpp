#include "TrackList.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>

TrackList::TrackList(const QVector<Track>& tracks, QWidget* parent) : QListWidget(parent)
{
    for (int i = 0; i < tracks.size(); ++i)
    {
        QListWidgetItem* qListWidgetItem = new QListWidgetItem(this);
        qListWidgetItem->setSizeHint(QSize(0, 50));
        qListWidgetItem->setData(Qt::UserRole, QVariant::fromValue(tracks[i]));
        this->setItemWidget(qListWidgetItem, createTrackItemWidget(tracks[i], i + 1));
    }

    this->setStyleSheet(R"(
        TrackList {
            background-color: #2d2d2d;
            border: 1px solid #3d3d3d;
            border-radius: 6px;
            padding: 2px;
            outline: none;
            font-family: 'Segoe UI', Roboto, sans-serif;
        }

        TrackList::item {
            border-bottom: 1px solid #383838;
            background-color: transparent;
        }

        TrackList::item:selected {
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

    this->setFrameShape(QFrame::NoFrame);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void TrackList::addTrack(const Track& track)
{
    QListWidgetItem* qListWidgetItem = new QListWidgetItem(this);
    qListWidgetItem->setSizeHint(QSize(0, 50));
    qListWidgetItem->setData(Qt::UserRole, QVariant::fromValue(track));
    setItemWidget(qListWidgetItem, createTrackItemWidget(track, count() + 1));
}

QWidget* TrackList::createTrackItemWidget(const Track& track, int index)
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

    this->updateItemWidgetStyle(itemWidget, false);
    itemWidget->installEventFilter(this);
    itemWidget->setAttribute(Qt::WA_Hover);

    itemWidget->setLayout(layout);

    return itemWidget;
}

void TrackList::updateItemWidgetStyle(QWidget* widget, bool hovered)
{
    widget->setProperty("isHovered", hovered);
    widget->setStyleSheet(QString(R"(
        QWidget {
            background-color: %1;
            border-radius: 4px;
        }
    )")
                              .arg(hovered ? "#383838" : "transparent"));
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}

bool TrackList::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::Enter)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget)
        {
            this->updateItemWidgetStyle(widget, true);
        }
    }
    else if (event->type() == QEvent::Leave)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (widget)
        {
            this->updateItemWidgetStyle(widget, false);
        }
    }
    return QListWidget::eventFilter(obj, event);
}

QString TrackList::formatDuration(qint64 milliseconds) const
{
    if (milliseconds <= 0)
        return "--:--";

    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}
