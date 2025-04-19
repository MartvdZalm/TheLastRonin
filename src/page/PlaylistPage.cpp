#include "PlaylistPage.h"

#include <QPixmap>
#include <QListWidgetItem>
#include <QPushButton>
#include "../components/CoverImageWidget.h"
#include "../playlist/PlaylistDetailsWidget.h"
#include "../playlist/TrackListWidget.h"
#include "../dialog/AddTrackDialog.h"
#include "../playlist/PlaylistManager.h"

PlaylistPage::PlaylistPage(const Playlist& playlist, QWidget* parent)
    : QWidget(parent), playlistData(playlist)
{
    this->setWindowTitle(playlist.name);
    this->setFixedSize(600, 500);

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);


    QPushButton* addTrackBtn = new QPushButton("Add Track");
    QHBoxLayout* addTrackRow = new QHBoxLayout;
    addTrackRow->addWidget(addTrackBtn);
    addTrackRow->addStretch();

    playlistData.tracks = PlaylistManager::instance().getTracksForPlaylist(playlist.id);

    TrackListWidget* trackListWidget = new TrackListWidget(playlistData.tracks, this);

    connect(addTrackBtn, &QPushButton::clicked, this, [=]() mutable {
        AddTrackDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Track newTrack = dialog.getTrack();

            PlaylistManager::instance().addTrackToPlaylist(playlistData.id, newTrack);

            trackListWidget->addTrack(newTrack);
            playlistData.tracks.append(newTrack);
        }
    });

    connect(trackListWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem* item) {
        int index = trackListWidget->row(item);
        const Track& track = playlistData.tracks[index];

        player->setSource(QUrl::fromLocalFile(track.filePath));
        audioOutput->setVolume(50);
        player->play();
    });


    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new CoverImageWidget(playlist.coverImagePath, this));
    layout->addWidget(new PlaylistDetailsWidget(playlist, this));
    layout->addLayout(addTrackRow);
    layout->addWidget(trackListWidget);
}
