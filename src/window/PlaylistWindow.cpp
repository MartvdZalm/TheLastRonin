#include "PlaylistWindow.h"
#include <QPixmap>
#include <QListWidgetItem>
#include <QTimer>
#include "../components/dialog/AddTrackDialog.h"
#include "../components/dialog/PlaylistDialog.h"
#include "../events/AppEvents.h"
#include "../components/shared/NavigationBar.h"
#include "MainWindow.h"

PlaylistWindow::PlaylistWindow(const Playlist& playlist, QWidget* parent)
    : BaseWindow(parent), playlistData(playlist), storedVolume(50), isMuted(false), miniPlayer(nullptr), isMiniPlayerActive(false)
{
    this->setStyle();
    this->setupUI();
    this->setupConnections();
}

PlaylistWindow::~PlaylistWindow()
{
    disconnect();

    if (player) {
        player->stop();
        player->deleteLater();
        player = nullptr;
    }

    if (audioOutput) {
        audioOutput->deleteLater();
        audioOutput = nullptr;
    }

    qDebug() << "PlaylistWindow destroyed safely";
}

void PlaylistWindow::setupUI()
{
    this->setWindowTitle(playlistData.name);
    this->showMaximized();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    NavigationBar* navBar = new NavigationBar(this);

    connect(navBar, &NavigationBar::backClicked, this, [this]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
            mainWindow->goBack();
        }
    });

    mainLayout->addWidget(navBar);

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

    playlistData.tracks = trackDAO.getTracksForPlaylist(playlistData.id);
    trackList = new TrackList(playlistData.tracks, this);

    coverImageWidget = new CoverImageWidget(playlistData.coverImagePath, this);
    detailsWidget = new PlaylistDetails(playlistData, this);

    mainLayout->addWidget(coverImageWidget);
    mainLayout->addWidget(detailsWidget);
    mainLayout->addLayout(optionsRow);
    mainLayout->addWidget(trackList, 1);
    mainLayout->addWidget(createPlayerBar());
}

void PlaylistWindow::setupConnections()
{
    connect(addTrackBtn, &QPushButton::clicked, this, [=]() {
        AddTrackDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Track newTrack = dialog.getTrack();

            trackDAO.insertTrack(playlistData.id, newTrack);

            trackList->addTrack(newTrack);
            playlistData.tracks.append(newTrack);
        }
    });

    connect(editPlaylistBtn, &QPushButton::clicked, this, [=]() {
        auto result = this->showEditPlaylistDialog();
        if (result.has_value()) {
            refreshMetadata(result.value());
            AppEvents::instance().notifyPlaylistChanged();
        }
    });

    connect(removePlaylistBtn, &QPushButton::clicked, this, [=]() {
        playlistDAO.deletePlaylist(playlistData.id);
        AppEvents::instance().notifyPlaylistChanged();
    });

    connect(trackList, &QListWidget::itemClicked, this, [=](QListWidgetItem* item) {
        int index = trackList->row(item);
        playTrackAtIndex(index);
    });

    connect(miniPlayerToggleButton, &QPushButton::clicked, this, &PlaylistWindow::toggleMiniPlayer);

    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
        if (!progressSlider->isSliderDown() && player->duration() > 0) {
            progressSlider->setValue(static_cast<int>(position));
            updateTimeLabel(position, player->duration(), timeLabel);
        }
        syncMiniPlayerControls();
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

    connect(volumeSlider, &QSlider::valueChanged, this, [=](int value) {
        if (!isMuted) {
            storedVolume = value;
        }
        audioOutput->setVolume(value / 100.0f);

        if (value == 0) {
            muteButton->setIcon(QIcon(":/Images/Mute"));
            isMuted = true;
        } else {
            muteButton->setIcon(QIcon(":/Images/Volume"));
            isMuted = false;
        }
    });

    connect(muteButton, &QPushButton::clicked, this, [=]() {
        if (isMuted) {
            audioOutput->setVolume(storedVolume / 100.0f);
            volumeSlider->setValue(storedVolume);
            muteButton->setIcon(QIcon(":/Images/Volume"));
            isMuted = false;
        } else {
            if (volumeSlider->value() > 0) {
                storedVolume = volumeSlider->value();
            }
            isMuted = true;
            audioOutput->setVolume(0);
            volumeSlider->setValue(0);
            muteButton->setIcon(QIcon(":/Images/Mute"));
        }
    });

    audioOutput->setVolume(storedVolume / 100.0f);
    volumeSlider->setValue(storedVolume);
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

void PlaylistWindow::setupEvents() {}

std::optional<Playlist> PlaylistWindow::showEditPlaylistDialog()
{
    PlaylistDialog dialog(playlistData, this);
    if (dialog.exec() == QDialog::Accepted) {
        Playlist playlist {
            .id = playlistData.id,
            .name = dialog.getName(),
            .description = dialog.getDescription(),
            .coverImagePath = dialog.getCoverImagePath(),
        };

        playlistDAO.updatePlaylist(playlist);
        return playlist;
    }

    return std::nullopt;
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

        trackList->setCurrentRow(index);
    }
    syncMiniPlayerControls();
}

void PlaylistWindow::playNextTrack()
{
    if (currentTrackIndex < playlistData.tracks.size() - 1) {
        playTrackAtIndex(currentTrackIndex + 1);
    } else {
        playTrackAtIndex(0);
    }
}

void PlaylistWindow::refreshMetadata(const Playlist& updatedPlaylist)
{
    playlistData.name = updatedPlaylist.name;
    playlistData.description = updatedPlaylist.description;
    playlistData.coverImagePath = updatedPlaylist.coverImagePath;

    setWindowTitle(playlistData.name);

    if (detailsWidget) {
        detailsWidget->updateDetails(playlistData);
    }

    if (coverImageWidget) {
        coverImageWidget->setImage(playlistData.coverImagePath);
    }
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

    muteButton = new QPushButton(this);
    muteButton->setIcon(QIcon(":/Images/Volume"));
    muteButton->setFixedSize(24, 24);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(storedVolume);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            background: #383838;
            height: 4px;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #4a90e2;
            width: 10px;
            height: 10px;
            margin: -3px 0;
            border-radius: 5px;
        }
        QSlider::sub-page:horizontal {
            background: #4a90e2;
            border-radius: 2px;
        }
    )");

    miniPlayerToggleButton = new QPushButton("📱", this);
    miniPlayerToggleButton->setFixedSize(24, 24);
    miniPlayerToggleButton->setToolTip("Open Mini Player");
    styleButton(miniPlayerToggleButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(playerBar);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(10);

    QHBoxLayout* controls = new QHBoxLayout();
    controls->addStretch();
    controls->addWidget(prevButton);
    controls->addWidget(pausePlayButton);
    controls->addWidget(nextButton);

    QHBoxLayout* volumeLayout = new QHBoxLayout();
    volumeLayout->addWidget(muteButton);
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->setSpacing(5);

    controls->addStretch();
    controls->addLayout(volumeLayout);
    controls->addWidget(miniPlayerToggleButton);

    QHBoxLayout* progressLayout = new QHBoxLayout();
    progressLayout->addWidget(songLabel, 1);
    progressLayout->addWidget(progressSlider, 4);
    progressLayout->addWidget(timeLabel, 1);

    mainLayout->addLayout(controls);
    mainLayout->addLayout(progressLayout);

    return playerBar;
}

void PlaylistWindow::toggleMiniPlayer()
{
    if (!isMiniPlayerActive) {
        miniPlayer = new MiniPlayerWindow(nullptr);
        miniPlayer->setPlayerData(player, audioOutput);

        if (currentTrackIndex >= 0 && currentTrackIndex < playlistData.tracks.size()) {
            miniPlayer->updateTrackInfo(playlistData.tracks[currentTrackIndex]);
        }

        miniPlayer->updatePlayPauseButton(player->playbackState() == QMediaPlayer::PlayingState);
        miniPlayer->updateProgress(player->position(), player->duration());

        connect(miniPlayer, &MiniPlayerWindow::playPauseClicked, this, [this]() {
            pausePlayButton->click();
        });

        connect(miniPlayer, &MiniPlayerWindow::nextClicked, this, [this]() {
            nextButton->click();
        });

        connect(miniPlayer, &MiniPlayerWindow::prevClicked, this, [this]() {
            prevButton->click();
        });

        connect(miniPlayer, &MiniPlayerWindow::progressChanged, this, [this](int position) {
            if (player->duration() > 0) {
                player->setPosition(position);
            }
        });

        connect(miniPlayer, &MiniPlayerWindow::volumeChanged, this, [this](int volume) {
            volumeSlider->setValue(volume);
        });

        connect(miniPlayer, &MiniPlayerWindow::miniPlayerClosed, this, &PlaylistWindow::onMiniPlayerClosed);

        miniPlayer->show();
        isMiniPlayerActive = true;
        miniPlayerToggleButton->setText("🔙");
        miniPlayerToggleButton->setToolTip("Close Mini Player");

        this->showMinimized();

    } else {
        if (miniPlayer) {
            miniPlayer->close();
            miniPlayer->deleteLater();
            miniPlayer = nullptr;
        }
        isMiniPlayerActive = false;
        miniPlayerToggleButton->setText("📱");
        miniPlayerToggleButton->setToolTip("Open Mini Player");

        this->showNormal();
        this->raise();
        this->activateWindow();
    }
}

void PlaylistWindow::onMiniPlayerClosed()
{
    isMiniPlayerActive = false;
    miniPlayerToggleButton->setText("📱");
    miniPlayerToggleButton->setToolTip("Open Mini Player");

    if (miniPlayer) {
        miniPlayer->deleteLater();
        miniPlayer = nullptr;
    }

    this->showNormal();
    this->raise();
    this->activateWindow();
}

void PlaylistWindow::syncMiniPlayerControls()
{
    if (miniPlayer && isMiniPlayerActive) {
        if (currentTrackIndex >= 0 && currentTrackIndex < playlistData.tracks.size()) {
            miniPlayer->updateTrackInfo(playlistData.tracks[currentTrackIndex]);
        }

        miniPlayer->updatePlayPauseButton(player->playbackState() == QMediaPlayer::PlayingState);
        miniPlayer->updateProgress(player->position(), player->duration());
    }
}
