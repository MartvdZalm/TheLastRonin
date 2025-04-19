#include "PlaylistPage.h"

#include <QPixmap>
#include <QListWidgetItem>

PlaylistPage::PlaylistPage(const Playlist& playlist, QWidget* parent)
    : QWidget(parent), playlistData(playlist)
{
    setWindowTitle(playlist.name);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Cover image
    coverImageLabel = new QLabel(this);
    QPixmap pixmap(playlist.coverImagePath);
    coverImageLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    coverImageLabel->setAlignment(Qt::AlignCenter);

    // Playlist details
    nameLabel = new QLabel("Name: " + playlist.name, this);
    descriptionLabel = new QLabel("Description: " + playlist.description, this);

    // Track list
    tracksListWidget = new QListWidget(this);
    for (const Track& track : playlist.tracks) {
        QListWidgetItem* item = new QListWidgetItem(track.title + " - " + track.artist);
        tracksListWidget->addItem(item);
    }

    // Add widgets to layout
    layout->addWidget(coverImageLabel);
    layout->addWidget(nameLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(tracksListWidget);

    setLayout(layout);
}
