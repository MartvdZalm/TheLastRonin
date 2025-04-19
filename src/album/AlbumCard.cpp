#include "AlbumCard.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QLabel>

AlbumCard::AlbumCard(const Album& album, QWidget* parent)
    : QWidget(parent), albumData(album) {

    const int cardWidth = 250;
    const int coverHeight = 200;
    const int titleHeight = 40;

    this->setFixedSize(cardWidth, coverHeight + titleHeight);

    QLabel* cover = new QLabel(this);
    cover->setFixedSize(cardWidth, coverHeight);
    cover->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(album.coverPath);
    cover->setPixmap(pixmap.scaled(cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    QLabel* titleLabel = new QLabel(album.title, this);
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

    previewLabel = new QLabel("Artist: " + album.artist + "\n Release Date: " + album.releaseDate, this);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("background-color: rgba(0, 0, 0, 0.7); color: white; padding: 5px;");
    previewLabel->setVisible(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(cover);
    layout->addWidget(titleLabel);
    layout->addWidget(previewLabel);

    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    cover->installEventFilter(this);

    this->setCursor(Qt::PointingHandCursor);
}

void AlbumCard::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit albumClicked(albumData);
}

bool AlbumCard::eventFilter(QObject* watched, QEvent* event)
{
    if (watched->inherits("QLabel")) {
        QLabel* cover = qobject_cast<QLabel*>(watched);

        if (event->type() == QEvent::Enter) {
            previewLabel->setVisible(true);
        } else if (event->type() == QEvent::Leave) {
            previewLabel->setVisible(false);
        }
    }

    return QWidget::eventFilter(watched, event);
}
