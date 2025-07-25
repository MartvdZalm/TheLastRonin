#include "PlaylistDetails.h"
#include <QVBoxLayout>

PlaylistDetails::PlaylistDetails(const Playlist& playlist, QWidget* parent)
    : QWidget(parent)
{
    this->setStyleSheet(R"(
        QLabel {
            font-weight: bold;
            color: white;
background-color: transparent;
        }

        #nameLabel {
            font-size: 24px;
        }
        #tracksLabel {
            font-size: 18px;
        }
    )");

    nameLabel = new QLabel(playlist.name, this);
    nameLabel->setObjectName("nameLabel");

    tracksLabel = new QLabel("Tracks: " + QString::number(playlist.tracks.size()));
    tracksLabel->setObjectName("tracksLabel");

    descriptionLabel = new QLabel(this);
    descriptionLabel->setText(playlist.description);
    descriptionLabel->setVisible(!playlist.description.isEmpty());

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(tracksLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void PlaylistDetails::updateDetails(const Playlist& playlist)
{
    nameLabel->setText(playlist.name);
    descriptionLabel->setText(playlist.description);
    tracksLabel->setText("Tracks: " + QString::number(playlist.tracks.size()));
}
