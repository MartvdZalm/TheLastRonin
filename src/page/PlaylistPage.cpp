#include "PlaylistPage.h"

#include <QPixmap>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTimer>
#include "../components/CoverImageWidget.h"
#include "../playlist/PlaylistDetailsWidget.h"
#include "../dialog/AddTrackDialog.h"
#include "../playlist/PlaylistManager.h"

PlaylistPage::PlaylistPage(const Playlist& playlist, QWidget* parent)
    : QWidget(parent), playlistData(playlist)
{
    setStyle();

    this->setWindowTitle(playlist.name);
    this->showMaximized();

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);

    QPushButton* addTrackBtn = new QPushButton("Add Track");
    QPushButton* editPlaylistBtn = new QPushButton("Edit Playlist");
    QHBoxLayout* addTrackRow = new QHBoxLayout;
    addTrackRow->addWidget(addTrackBtn);
    addTrackRow->addWidget(editPlaylistBtn);
    addTrackRow->addStretch();

    playlistData.tracks = PlaylistManager::instance().getTracksForPlaylist(playlist.id);
    trackListWidget = new TrackListWidget(playlistData.tracks, this);

    connect(addTrackBtn, &QPushButton::clicked, this, [=]() mutable {
        AddTrackDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Track newTrack = dialog.getTrack();
            PlaylistManager::instance().addTrackToPlaylist(playlistData.id, newTrack);
            trackListWidget->addTrack(newTrack);
            playlistData.tracks.append(newTrack);
        }
    });

    connect(editPlaylistBtn, &QPushButton::clicked, this, [=]() {
        PlaylistManager::instance().openEditPlaylistDialog(playlistData);
    });

    songLabel = new QLabel("No song playing", this);
    songLabel->setStyleSheet(R"(
        QLabel {
            color: #4a90e2;
            font-size: 16px;
            font-weight: bold;
            padding-left: 10px;
        }
    )");
    songLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


    QSlider* progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);


    QLabel* timeLabel = new QLabel("00:00 / 00:00", this);
    timeLabel->setStyleSheet(R"(
        QLabel {
            color: #888;
            font-size: 12px;
            padding-right: 10px;
        }
    )");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);


    QPushButton* pausePlayButton = new QPushButton(this);
    QPushButton* prevButton = new QPushButton(this);
    QPushButton* nextButton = new QPushButton(this);
    pausePlayButton->setIcon(QIcon(":/Images/Pause"));
    prevButton->setIcon(QIcon(":/Images/Prev"));
    nextButton->setIcon(QIcon(":/Images/Next"));
    pausePlayButton->setObjectName("iconButton");
    prevButton->setObjectName("iconButton");
    nextButton->setObjectName("iconButton");

    QWidget* playerBar = new QWidget(this);
    playerBar->setStyleSheet(R"(
        QWidget {
            background-color: #2a2a2a;
            border-radius: 6px;
            padding: 8px;
        }
    )");

    QVBoxLayout* playerBarMainLayout = new QVBoxLayout(playerBar);
    playerBarMainLayout->setContentsMargins(5, 5, 5, 5);
    playerBarMainLayout->setSpacing(10);

    QHBoxLayout* controlsLayout = new QHBoxLayout();
    controlsLayout->setContentsMargins(0, 0, 0, 0);
    controlsLayout->addStretch();
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(pausePlayButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addStretch();

    QHBoxLayout* progressInfoLayout = new QHBoxLayout();
    progressInfoLayout->addWidget(songLabel, 1);
    progressInfoLayout->addWidget(progressSlider, 4);
    progressInfoLayout->addWidget(timeLabel, 1);

    playerBarMainLayout->addLayout(controlsLayout);
    playerBarMainLayout->addLayout(progressInfoLayout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);


    CoverImageWidget* coverImage = new CoverImageWidget(playlist.coverImagePath, this);
    coverImage->setStyleSheet(R"(
        CoverImageWidget {
            border-radius: 8px;
            background: transparent;
        }
    )");


    PlaylistDetailsWidget* detailsWidget = new PlaylistDetailsWidget(playlist, this);

    layout->addWidget(coverImage);
    layout->addWidget(detailsWidget);
    layout->addLayout(addTrackRow);
    layout->addWidget(trackListWidget, 1);
    layout->addWidget(playerBar);


    connect(trackListWidget, &QListWidget::itemClicked, this, [=](QListWidgetItem* item) {
        int index = trackListWidget->row(item);
        playTrackAtIndex(index);
    });

    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
        if (!progressSlider->isSliderDown() && player->duration() > 0) {
            progressSlider->setValue(static_cast<int>(position));
            updateTimeLabel(position, player->duration(), timeLabel);
        }
    });

    connect(player, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        progressSlider->setMaximum(static_cast<int>(duration));
        updateTimeLabel(player->position(), duration, timeLabel);
    });

    connect(progressSlider, &QSlider::sliderReleased, this, [=]() {
        if (player->duration() > 0) {
            player->setPosition(progressSlider->value());
        }
    });

    connect(progressSlider, &QSlider::sliderMoved, this, [=](int position) {
        updateTimeLabel(position, player->duration(), timeLabel);
    });

    connect(pausePlayButton, &QPushButton::clicked, this, [this, pausePlayButton]() {
        if (player->playbackState() == QMediaPlayer::PlayingState) {
            player->pause();
            pausePlayButton->setIcon(QIcon(":/Images/Play"));
        } else {
            player->play();
            pausePlayButton->setIcon(QIcon(":/Images/Pause"));
        }
    });

    connect(prevButton, &QPushButton::clicked, this, [=]() {
        if (currentTrackIndex > 0) {
            currentTrackIndex--;
            playTrackAtIndex(currentTrackIndex);
        }
    });

    connect(nextButton, &QPushButton::clicked, this, [=]() {
        if (currentTrackIndex < playlistData.tracks.size() - 1) {
            currentTrackIndex++;
            playTrackAtIndex(currentTrackIndex);
        }
    });

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            playNextTrack();
        }
    });
}

void PlaylistPage::updateTimeLabel(qint64 position, qint64 duration, QLabel* label)
{
    int seconds = position / 1000;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int totalSeconds = duration / 1000;
    int totalMinutes = totalSeconds / 60;
    totalSeconds = totalSeconds % 60;

    label->setText(QString("%1:%2 / %3:%4")
       .arg(minutes, 2, 10, QChar('0'))
       .arg(seconds, 2, 10, QChar('0'))
       .arg(totalMinutes, 2, 10, QChar('0'))
       .arg(totalSeconds, 2, 10, QChar('0')));
}

void PlaylistPage::playTrackAtIndex(int index)
{
    if (index >= 0 && index < playlistData.tracks.size()) {
        currentTrackIndex = index;
        const Track& track = playlistData.tracks[index];

        songLabel->setText(track.title);

        player->setSource(QUrl::fromLocalFile(track.filePath));
        player->play();

        trackListWidget->setCurrentRow(index);

        QTimer::singleShot(100, this, [=]() {
            player->setSource(QUrl::fromLocalFile(track.filePath));
            player->play();
        });
    }
}

void PlaylistPage::playNextTrack()
{
    if (currentTrackIndex < playlistData.tracks.size() - 1) {
        playTrackAtIndex(currentTrackIndex + 1);
    } else {
        playTrackAtIndex(0);
    }
}

void PlaylistPage::setStyle()
{
    this->setStyleSheet(R"(
        QWidget {
            background-color: #252525;
            color: #e0e0e0;
            font-family: 'Segoe UI', Roboto, sans-serif;
            border: none;
            outline: none;
        }

        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
            min-width: 80px;
        }

        QPushButton:hover {
            background-color: #5aa0f2;
        }

        QPushButton:pressed {
            background-color: #3a80d2;
        }

        QLabel {
            color: #e0e0e0;
        }

        QSlider {
            height: 6px;
        }

        QSlider::groove:horizontal {
            background: #383838;
            height: 6px;
            border-radius: 3px;
        }

        QSlider::handle:horizontal {
            background: #4a90e2;
            width: 12px;
            height: 12px;
            margin: -3px 0;
            border-radius: 6px;
        }

        QSlider::sub-page:horizontal {
            background: #4a90e2;
            border-radius: 3px;
        }
    )");
}
