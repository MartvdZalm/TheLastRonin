#include "PlaylistDetails.h"
#include <QVBoxLayout>

PlaylistDetails::PlaylistDetails(const Playlist& playlist, QWidget* parent)
    : QWidget(parent)
{
    this->setStyleSheet(R"(
        QLabel {
            font-weight: bold;
            color: white;
        }

        #nameLabel {
            font-size: 24px;
        }
        #tracksLabel {
            font-size: 18px;
        }
    )");

    nameLabel = new QLabel("Name: " + playlist.name, this);
    nameLabel->setObjectName("nameLabel");

    if (playlist.description.isEmpty()) {

    }
    tracksLabel = new QLabel("Tracks: " + QString::number(playlist.tracks.size()));
    tracksLabel->setObjectName("tracksLabel");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);

    if (!playlist.description.isEmpty()) {
        descriptionLabel = new QLabel(playlist.description, this);
        layout->addWidget(descriptionLabel);
    }

    layout->addWidget(tracksLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}
