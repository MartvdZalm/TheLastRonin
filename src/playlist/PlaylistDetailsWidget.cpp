#include "PlaylistDetailsWidget.h"
#include <QVBoxLayout>

PlaylistDetailsWidget::PlaylistDetailsWidget(const Playlist& playlist, QWidget* parent)
    : QWidget(parent)
{
    nameLabel = new QLabel("Name: " + playlist.name, this);
    descriptionLabel = new QLabel("Description: " + playlist.description, this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(descriptionLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}
