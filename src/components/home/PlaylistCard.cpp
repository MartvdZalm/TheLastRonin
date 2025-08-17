#include "PlaylistCard.h"

#include "../../events/AppEvents.h"
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QVBoxLayout>
#include <iostream>

PlaylistCard::PlaylistCard(const Playlist& playlist, QWidget* parent) : QWidget(parent)
{
    this->playlist = playlist;

    const int cardWidth = 250;
    const int coverHeight = 200;
    const int titleHeight = 40;

    this->setFixedSize(cardWidth, coverHeight + titleHeight);

    QLabel* cover = new QLabel(this);
    cover->setFixedSize(cardWidth, coverHeight);
    cover->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(playlist.getCoverImagePath());
    cover->setPixmap(pixmap.scaled(cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    QLabel* titleLabel = new QLabel(playlist.getName(), this);
    titleLabel->setToolTip(playlist.getName());
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(titleHeight);
    titleLabel->setStyleSheet(R"(
        QLabel {
            background-color: #333;
            color: white;
            font-size: 20px;
        }
    )");
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleLabel->setWordWrap(false);
    titleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    QFontMetrics metrics(titleLabel->font());
    QString elidedText = metrics.elidedText(playlist.getName(), Qt::ElideRight, titleLabel->width() - 2);
    titleLabel->setText(elidedText);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(cover);
    layout->addWidget(titleLabel);

    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->setCursor(Qt::PointingHandCursor);
}

void PlaylistCard::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    AppEvents::instance().notifyNavigateToPlaylist(playlist);
}
