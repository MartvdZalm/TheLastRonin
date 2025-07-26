#include "PlaybackBar.h"

#include "../../styles/ButtonStyle.h"
#include <QTimer>

PlaybackBar::PlaybackBar(QWidget* parent)
    : storedVolume(50), isMuted(false), miniPlayer(nullptr), isMiniPlayerActive(false)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("playbackBar");
    this->setStyleSheet(R"(
        #playbackBar {
            background-color: #2a2a2a;
            border-radius: 6px;
            padding: 8px;
        }
    )");

    this->setupUI();
    this->setupConnections();
}

PlaybackBar::~PlaybackBar()
{
    if (player)
    {
        player->stop();
        player->deleteLater();
        player = nullptr;
    }

    if (audioOutput)
    {
        audioOutput->deleteLater();
        audioOutput = nullptr;
    }
}

PlaybackBar* PlaybackBar::instance()
{
    static PlaybackBar* _instance = new PlaybackBar();
    return _instance;
}

void PlaybackBar::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 15, 5, 15);

    QHBoxLayout* controls = new QHBoxLayout();
    controls->setContentsMargins(0, 0, 0, 0);

    prevButton = new QPushButton(this);
    prevButton->setIcon(QIcon(":/Images/Prev"));
    prevButton->setStyleSheet(ButtonStyle::primary());

    pausePlayButton = new QPushButton(this);
    pausePlayButton->setIcon(QIcon(":/Images/Pause"));
    pausePlayButton->setStyleSheet(ButtonStyle::primary());

    nextButton = new QPushButton(this);
    nextButton->setIcon(QIcon(":/Images/Next"));
    nextButton->setStyleSheet(ButtonStyle::primary());

    controls->addStretch();
    controls->addWidget(prevButton);
    controls->addWidget(pausePlayButton);
    controls->addWidget(nextButton);
    controls->addStretch();

    QHBoxLayout* centerRow = new QHBoxLayout();
    centerRow->setContentsMargins(0, 0, 0, 0);

    songLabel = new QLabel("No song playing", this);
    songLabel->setStyleSheet(
        "background-color: #2a2a2a; color: #4a90e2; font-size: 16px; font-weight: bold; padding-left: 10px;");
    songLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout* volumeLayout = new QHBoxLayout();

    muteButton = new QPushButton(this);
    muteButton->setIcon(QIcon(":/Images/Volume"));
    muteButton->setFixedSize(24, 24);
    muteButton->setStyleSheet(ButtonStyle::primary());

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(storedVolume);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setStyleSheet(R"(
        QSlider {
            background-color: #2a2a2a;
        }

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

    miniPlayerToggleButton = new QPushButton(this);
    miniPlayerToggleButton->setIcon(QIcon(":/Images/PictureInPicture"));
    miniPlayerToggleButton->setFixedSize(24, 24);
    miniPlayerToggleButton->setToolTip("Open Mini Player");
    miniPlayerToggleButton->setStyleSheet(ButtonStyle::primary());

    volumeLayout->addWidget(muteButton);
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->addWidget(miniPlayerToggleButton);

    centerRow->addWidget(songLabel);
    centerRow->addLayout(volumeLayout);

    QHBoxLayout* progressLayout = new QHBoxLayout();
    progressLayout->setContentsMargins(0, 0, 0, 0);

    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(50);

    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);
    progressSlider->setStyleSheet(R"(
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

    timeLabel = new QLabel("00:00 / 00:00", this);
    timeLabel->setStyleSheet("background-color: #2a2a2a; color: #888; font-size: 12px; padding-right: 10px;");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    progressLayout->addStretch();
    progressLayout->addWidget(progressSlider);
    progressLayout->addWidget(timeLabel);
    progressLayout->addStretch();

    mainLayout->addLayout(controls);
    mainLayout->addLayout(centerRow);
    mainLayout->addLayout(progressLayout);
}

void PlaybackBar::setupConnections()
{
    connect(miniPlayerToggleButton, &QPushButton::clicked, this, &PlaybackBar::toggleMiniPlayer);

    connect(player, &QMediaPlayer::positionChanged, this,
            [=](qint64 position)
            {
                if (!progressSlider->isSliderDown() && player->duration() > 0)
                {
                    progressSlider->setValue(static_cast<int>(position));
                    updateTimeLabel(position, player->duration(), timeLabel);
                }
                syncMiniPlayerControls();
            });

    connect(player, &QMediaPlayer::durationChanged, this,
            [=](qint64 duration)
            {
                progressSlider->setMaximum(static_cast<int>(duration));
                updateTimeLabel(player->position(), duration, timeLabel);
            });

    connect(progressSlider, &QSlider::sliderReleased, this,
            [=]()
            {
                if (player->duration() > 0)
                {
                    player->setPosition(progressSlider->value());
                }
            });

    connect(progressSlider, &QSlider::sliderMoved, this,
            [=](int position) { updateTimeLabel(position, player->duration(), timeLabel); });

    connect(pausePlayButton, &QPushButton::clicked, this,
            [=]()
            {
                if (player->playbackState() == QMediaPlayer::PlayingState)
                {
                    player->pause();
                    pausePlayButton->setIcon(QIcon(":/Images/Play"));
                }
                else
                {
                    player->play();
                    pausePlayButton->setIcon(QIcon(":/Images/Pause"));
                }

                if (isMiniPlayerActive)
                {
                    miniPlayer->updatePlayPauseButton(player->playbackState() == QMediaPlayer::PlayingState);
                }
            });

    connect(prevButton, &QPushButton::clicked, this,
            [=]()
            {
                if (currentTrackIndex > 0)
                {
                    currentTrackIndex--;
                    playTrackAtIndex(currentTrackIndex);
                }
                onPrevClicked();
            });

    connect(nextButton, &QPushButton::clicked, this,
            [=]()
            {
                if (currentTrackIndex < playlist.tracks.size() - 1)
                {
                    currentTrackIndex++;
                    playTrackAtIndex(currentTrackIndex);
                }
                onNextClicked();
            });

    connect(player, &QMediaPlayer::mediaStatusChanged, this,
            [=](QMediaPlayer::MediaStatus status)
            {
                if (status == QMediaPlayer::EndOfMedia)
                {
                    playNextTrack();
                }
            });

    connect(volumeSlider, &QSlider::valueChanged, this,
            [=](int value)
            {
                if (!isMuted)
                {
                    storedVolume = value;
                }
                audioOutput->setVolume(value / 100.0f);

                if (value == 0)
                {
                    muteButton->setIcon(QIcon(":/Images/Mute"));
                    isMuted = true;
                }
                else
                {
                    muteButton->setIcon(QIcon(":/Images/Volume"));
                    isMuted = false;
                }
            });

    connect(muteButton, &QPushButton::clicked, this,
            [=]()
            {
                if (isMuted)
                {
                    audioOutput->setVolume(storedVolume / 100.0f);
                    volumeSlider->setValue(storedVolume);
                    muteButton->setIcon(QIcon(":/Images/Volume"));
                    isMuted = false;
                }
                else
                {
                    if (volumeSlider->value() > 0)
                    {
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

void PlaybackBar::toggleMiniPlayer()
{
    if (!isMiniPlayerActive)
    {
        miniPlayer = new MiniPlayerWindow(nullptr);
        miniPlayer->setPlayerData(player, audioOutput);

        if (currentTrackIndex >= 0 && currentTrackIndex < playlist.tracks.size())
        {
            miniPlayer->updateTrackInfo(playlist.tracks[currentTrackIndex]);
        }

        miniPlayer->updatePlayPauseButton(player->playbackState() == QMediaPlayer::PlayingState);
        miniPlayer->updateProgress(player->position(), player->duration());
        miniPlayer->updateVolumeSlider(volumeSlider->value());

        connect(miniPlayer, &MiniPlayerWindow::playPauseClicked, this, [this]() { pausePlayButton->click(); });

        connect(miniPlayer, &MiniPlayerWindow::nextClicked, this, [this]() { nextButton->click(); });

        connect(miniPlayer, &MiniPlayerWindow::prevClicked, this, [this]() { prevButton->click(); });

        connect(miniPlayer, &MiniPlayerWindow::progressChanged, this,
                [this](int position)
                {
                    if (player->duration() > 0)
                    {
                        player->setPosition(position);
                    }
                });

        connect(miniPlayer, &MiniPlayerWindow::volumeChanged, this,
                [this](int volume) { volumeSlider->setValue(volume); });

        connect(miniPlayer, &MiniPlayerWindow::miniPlayerClosed, this, &PlaybackBar::onMiniPlayerClosed);

        miniPlayer->show();
        isMiniPlayerActive = true;
        miniPlayerToggleButton->setIcon(QIcon(":/Images/PictureInPictureClose"));
        miniPlayerToggleButton->setToolTip("Close Mini Player");

        this->showMinimized();
    }
    else
    {
        if (miniPlayer)
        {
            miniPlayer->close();
            miniPlayer->deleteLater();
            miniPlayer = nullptr;
        }
        isMiniPlayerActive = false;
        miniPlayerToggleButton->setIcon(QIcon(":/Images/PictureInPicture"));
        miniPlayerToggleButton->setToolTip("Open Mini Player");

        this->showNormal();
        this->raise();
        this->activateWindow();
    }
}

void PlaybackBar::onMiniPlayerClosed()
{
    isMiniPlayerActive = false;
    miniPlayerToggleButton->setIcon(QIcon(":/Images/PictureInPicture"));
    miniPlayerToggleButton->setToolTip("Open Mini Player");

    if (miniPlayer)
    {
        miniPlayer->close();
    }

    this->showNormal();
    this->raise();
    this->activateWindow();
}

void PlaybackBar::syncMiniPlayerControls()
{
    if (miniPlayer && isMiniPlayerActive)
    {
        if (currentTrackIndex >= 0 && currentTrackIndex < playlist.tracks.size())
        {
            miniPlayer->updateTrackInfo(playlist.tracks[currentTrackIndex]);
        }

        miniPlayer->updatePlayPauseButton(player->playbackState() == QMediaPlayer::PlayingState);
        miniPlayer->updateProgress(player->position(), player->duration());
    }
}

void PlaybackBar::updateTimeLabel(qint64 position, qint64 duration, QLabel* label)
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

void PlaybackBar::playNextTrack()
{
    if (currentTrackIndex < playlist.tracks.size() - 1)
    {
        playTrackAtIndex(currentTrackIndex + 1);
    }
    else
    {
        playTrackAtIndex(0);
    }
}

void PlaybackBar::updatePlaylist(const Playlist& playlist)
{
    this->playlist = playlist;
}

void PlaybackBar::playTrackAtIndex(int index)
{
    if (index >= 0 && index < playlist.tracks.size())
    {
        currentTrackIndex = index;
        this->currentTrack = playlist.tracks[index];

        songLabel->setText(this->currentTrack.title);

        player->setSource(QUrl::fromLocalFile(this->currentTrack.filePath));
        player->play();

        QTimer::singleShot(100, this,
                           [=]()
                           {
                               player->setSource(QUrl::fromLocalFile(this->currentTrack.filePath));
                               player->play();
                           });
    }

    this->syncMiniPlayerControls();
}

void PlaybackBar::onNextClicked()
{
    emit nextClicked();
}

void PlaybackBar::onPrevClicked()
{
    emit prevClicked();
}
