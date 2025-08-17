#include "TrackList.h"

#include "../../constants/Colors.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QToolButton>
#include <QVBoxLayout>

TrackList::TrackList(const QList<Track>& tracks, QWidget* parent, bool showMenu)
    : QListWidget(parent), showMenu(showMenu)
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
        }

        TrackList::item {
            border-bottom: 1px solid #383838;
            background-color: transparent;
        }

        TrackList::item:selected {
            background-color: #4a90e2;
            border-radius: 4px;
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
    setItemWidget(qListWidgetItem, createTrackItemWidget(track, count()));
}

void TrackList::updateTracks(const QList<Track>& tracks)
{
    clear();
    for (int i = 0; i < tracks.size(); ++i)
    {
        QListWidgetItem* qListWidgetItem = new QListWidgetItem(this);
        qListWidgetItem->setSizeHint(QSize(0, 60));
        qListWidgetItem->setData(Qt::UserRole, QVariant::fromValue(tracks[i]));
        this->setItemWidget(qListWidgetItem, createTrackItemWidget(tracks[i], i + 1));
    }
}

QWidget* TrackList::createTrackItemWidget(const Track& track, int index)
{
    QWidget* itemWidget = new QWidget(this);
    itemWidget->setProperty("isHovered", false);

    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(15);

    QLabel* numberLabel = new QLabel(QString::number(index), itemWidget);
    numberLabel->setStyleSheet("color: #888; font-size: 14px;");
    layout->addWidget(numberLabel);

    QLabel* titleLabel = new QLabel(track.getTitle(), itemWidget);
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel, 1);

    QLabel* durationLabel = new QLabel(formatDuration(track.getDuration()), itemWidget);
    durationLabel->setStyleSheet("color: #888; font-size: 14px;");
    layout->addWidget(durationLabel);

    if (showMenu)
    {
        QToolButton* menuButton = new QToolButton(itemWidget);
        menuButton->setIcon(QIcon(":/Images/MenuDots"));
        menuButton->setStyleSheet("QToolButton::menu-indicator { image: none; }");
        menuButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

        QMenu* menu = new QMenu(menuButton);
        menu->setAttribute(Qt::WA_TranslucentBackground);
        menu->setAttribute(Qt::WA_OpaquePaintEvent);
        menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        menu->setWindowFlags(menu->windowFlags() | Qt::Popup | Qt::Tool);
        menu->setStyleSheet(QString(R"(
            QMenu {
                background-color: %1;
                border-radius: 8px;
                width: 200px;
                border: none
            }
            QMenu::item {
                padding: 8px 16px;
                background: transparent;
                width: 168px;
                border-radius: 8px;
            }
            QMenu::item:selected {
                background-color: #383838;
            }
        )")
                                .arg(Colors::DarkGray));
        menu->addAction("Play Next", [track]() { qDebug() << "Play next:" << track.getTitle(); });
        menu->addAction("Download", [track]() { qDebug() << "Download:" << track.getTitle(); });
        menu->addAction("Add to Playlist", [track]() { qDebug() << "Add to playlist:" << track.getTitle(); });

        connect(menu, &QMenu::aboutToShow, this,
                [this, menu]()
                {
                    if (currentMenu && currentMenu != menu)
                    {
                        currentMenu->close();
                    }
                    currentMenu = menu;
                });

        connect(menu, &QMenu::aboutToHide, this,
                [this, menu]()
                {
                    if (currentMenu == menu)
                    {
                        currentMenu = nullptr;
                    }
                });

        menuButton->setMenu(menu);
        menuButton->setPopupMode(QToolButton::InstantPopup);
        layout->addWidget(menuButton);
    }

    this->updateItemWidgetStyle(itemWidget, false);

    itemWidget->installEventFilter(this);
    itemWidget->setAttribute(Qt::WA_Hover);
    itemWidget->setLayout(layout);

    return itemWidget;
}

void TrackList::updateItemWidgetStyle(QWidget* widget, bool hovered)
{
    widget->setProperty("isHovered", hovered);
    widget->setStyleSheet(
        QString("background-color: %1; border-radius: 4px;").arg(hovered ? "#383838" : "transparent"));
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

    if (obj == currentMenu && event->type() == QEvent::Hide)
    {
        currentMenu = nullptr;
    }

    return QListWidget::eventFilter(obj, event);
}

QString TrackList::formatDuration(qint64 seconds) const
{
    if (seconds <= 0)
        return "--:--";

    int minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}
