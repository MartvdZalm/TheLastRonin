#include "PlaylistWindow.h"

#include <QPixmap>
#include <QListWidgetItem>
#include <QTimer>
#include "../components/CoverImageWidget.h"
#include "../playlist/PlaylistDetailsWidget.h"
#include "../dialog/AddTrackDialog.h"
#include "../playlist/PlaylistManager.h"

PlaylistWindow::PlaylistWindow(const Playlist& playlist, QWidget* parent)
    : BaseWindow(parent), playlistData(playlist)
{
    setStyle();
    setupUI();
    setupConnections();
}

void PlaylistWindow::setupUI()
{
    this->setWindowTitle(playlistData.name);
    this->showMaximized();

    initPlayer();
    createControlButtons();

    addTrackBtn = new QPushButton("Add Track");
    editPlaylistBtn = new QPushButton("Edit Playlist");
    removePlaylistBtn = new QPushButton("Delete Playlist");
    styleButton(removePlaylistBtn);
    removePlaylistBtn->setStyleSheet("background-color: red;");

    QHBoxLayout* optionsRow = new QHBoxLayout;
    optionsRow->addWidget(addTrackBtn);
    optionsRow->addWidget(editPlaylistBtn);
    optionsRow->addWidget(removePlaylistBtn);
    optionsRow->addStretch();

    playlistData.tracks = PlaylistManager::instance().getTracksForPlaylist(playlistData.id);
    trackListWidget = new TrackListWidget(playlistData.tracks, this);

    CoverImageWidget* coverImage = new CoverImageWidget(playlistData.coverImagePath, this);
    PlaylistDetailsWidget* detailsWidget = new PlaylistDetailsWidget(playlistData, this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    layout->addWidget(coverImage);
    layout->addWidget(detailsWidget);
    layout->addLayout(optionsRow);
    layout->addWidget(trackListWidget, 1);
    layout->addWidget(createPlayerBar());
}

void PlaylistWindow::setupConnections()
{
    connect(addTrackBtn, &QPushButton::clicked, this, [=]() {
        AddTrackDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Track newTrack = dialog.getTrack();
            PlaylistManager::instance().addTrackToPlaylist(playlistData.id, newTrack);
            trackListWidget->addTrack(newTrack);
            playlistData.tracks.append(newTrack);
        }
    });

    connect(editPlaylistBtn, &QPushButton::clicked, this, [=]() {
        auto result = PlaylistManager::instance().openEditPlaylistDialog(playlistData);
        if (result.has_value()) {
            loadPlaylist(result.value());
        }
    });

    connect(removePlaylistBtn, &QPushButton::clicked, this, [=]() {
        PlaylistManager::instance().removePlaylist(playlistData.id);
        close();
    });

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

    connect(pausePlayButton, &QPushButton::clicked, this, [=]() {
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

void PlaylistWindow::reload()
{
    loadPlaylist(playlistData);
}

void PlaylistWindow::setStyle()
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

void PlaylistWindow::updateTimeLabel(qint64 position, qint64 duration, QLabel* label)
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

void PlaylistWindow::playTrackAtIndex(int index)
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

void PlaylistWindow::playNextTrack()
{
    if (currentTrackIndex < playlistData.tracks.size() - 1) {
        playTrackAtIndex(currentTrackIndex + 1);
    } else {
        playTrackAtIndex(0);
    }
}

void PlaylistWindow::loadPlaylist(const Playlist& playlist)
{
    playlistData = playlist;

    QLayout* oldLayout = layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }

    setupUI();
    setupConnections();
}

void PlaylistWindow::initPlayer()
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);
}

void PlaylistWindow::createControlButtons()
{
    pausePlayButton = new QPushButton(this);
    prevButton = new QPushButton(this);
    nextButton = new QPushButton(this);

    styleButton(pausePlayButton, ":/Images/Pause");
    styleButton(prevButton, ":/Images/Prev");
    styleButton(nextButton, ":/Images/Next");
}

void PlaylistWindow::styleButton(QPushButton* button, const QString& iconPath)
{
    if (!iconPath.isEmpty()) {
        button->setIcon(QIcon(iconPath));
    }
    button->setObjectName("iconButton");
}

QWidget* PlaylistWindow::createPlayerBar()
{
    QWidget* playerBar = new QWidget(this);
    playerBar->setStyleSheet(R"(
        QWidget {
            background-color: #2a2a2a;
            border-radius: 6px;
            padding: 8px;
        }
    )");

    songLabel = new QLabel("No song playing", this);
    songLabel->setStyleSheet("color: #4a90e2; font-size: 16px; font-weight: bold; padding-left: 10px;");
    songLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);

    timeLabel = new QLabel("00:00 / 00:00", this);
    timeLabel->setStyleSheet("color: #888; font-size: 12px; padding-right: 10px;");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QVBoxLayout* mainLayout = new QVBoxLayout(playerBar);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);

    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    controls->addWidget(prevButton);
    controls->addWidget(pausePlayButton);
    controls->addWidget(nextButton);
    controls->addStretch();

    QHBoxLayout* progressLayout = new QHBoxLayout();
    progressLayout->addWidget(songLabel, 1);
    progressLayout->addWidget(progressSlider, 4);
    progressLayout->addWidget(timeLabel, 1);

    mainLayout->addLayout(controls);
    mainLayout->addLayout(progressLayout);

    return playerBar;
}
