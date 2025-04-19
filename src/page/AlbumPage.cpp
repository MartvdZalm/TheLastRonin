#include "AlbumPage.h"
#include <QBoxLayout>
#include <QLabel>


AlbumPage::AlbumPage(const Album& album, QWidget* parent)
    : QWidget(parent), albumData(album)
{
    setWindowTitle(album.title);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* cover = new QLabel(this);
    QPixmap pixmap(album.coverPath);
    cover->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    cover->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("Title: " + album.title, this);
    QLabel* artist = new QLabel("Artist: " + album.artist, this);
    QLabel* year = new QLabel("Year: " + album.releaseDate, this);

    layout->addWidget(cover);
    layout->addWidget(title);
    layout->addWidget(artist);
    layout->addWidget(year);
}
