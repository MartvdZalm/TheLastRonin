#include "MiniPlayer.h"

#include "../../styles/ButtonStyle.h"
#include "../../styles/SliderStyle.h"
#include "PlaybackBar.h"
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>

MiniPlayer::MiniPlayer(QWidget* parent)
    : QWidget(parent), playerRef(nullptr), audioOutputRef(nullptr), isDragging(false)
{
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    this->setMaximumWidth(300);
    this->setMinimumHeight(200);

    this->setupUI();
    this->setupConnections();

    QScreen* screen = QApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    this->move(screenRect.width() - width() - 20, screenRect.height() - height() - 50);
}

MiniPlayer::~MiniPlayer() {}

void MiniPlayer::setupUI()
{
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet(R"(
        #contentWidget {
            background-color: rgba(42, 42, 42, 230);
            border-radius: 12px;
        }
    )");

    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(12, 12, 12, 12);
    contentLayout->setSpacing(8);

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setAlignment(Qt::AlignRight);

    toggleVideoButton = new QPushButton(this);
    toggleVideoButton->setFixedSize(20, 20);
    toggleVideoButton->setToolTip(tr("Toggle Video/Thumbnail"));
    toggleVideoButton->setStyleSheet(ButtonStyle::primary());

    closeButton = new QPushButton(this);
    closeButton->setFixedSize(20, 20);
    closeButton->setIcon(QIcon(":/Images/Close"));
    closeButton->setStyleSheet(ButtonStyle::primary());

    topLayout->addWidget(toggleVideoButton);
    topLayout->addWidget(closeButton);

    mediaDisplayWidget = new QWidget(this);
    mediaDisplayWidget->setFixedSize(250, 140);
    mediaDisplayWidget->setStyleSheet("background-color: #222; border-radius: 8px;");

    QVBoxLayout* mediaLayout = new QVBoxLayout(mediaDisplayWidget);
    mediaLayout->setContentsMargins(0, 0, 0, 0);

    thumbnailLabel = new QLabel(mediaDisplayWidget);
    thumbnailLabel->setAlignment(Qt::AlignCenter);
    thumbnailLabel->setStyleSheet(R"(
        QLabel {
            background-color: #333;
            border-radius: 8px;
            color: #888;
            font-size: 24px;
        }
    )");
    thumbnailLabel->setText("♪");
    thumbnailLabel->setScaledContents(true);

    videoWidget = new QVideoWidget(mediaDisplayWidget);
    videoWidget->setStyleSheet("border-radius: 8px; background-color: #222;");
    videoWidget->hide();

    mediaLayout->addWidget(thumbnailLabel);
    mediaLayout->addWidget(videoWidget);

    songLabel = new QLabel(tr("No song playing"), this);
    songLabel->setAlignment(Qt::AlignCenter);
    songLabel->setWordWrap(true);
    songLabel->setMaximumHeight(35);
    songLabel->setStyleSheet("color: white; font-size: 12px; font-weight: bold;");

    QHBoxLayout* controlsLayout = new QHBoxLayout;
    controlsLayout->setSpacing(8);

    prevButton = new QPushButton(this);
    prevButton->setIcon(QIcon(":/Images/Prev"));
    prevButton->setFixedSize(25, 25);
    prevButton->setStyleSheet(ButtonStyle::primary());

    playPauseButton = new QPushButton(this);
    playPauseButton->setIcon(QIcon(":/Images/Pause"));
    playPauseButton->setFixedSize(25, 25);
    playPauseButton->setStyleSheet(ButtonStyle::primary());

    nextButton = new QPushButton(this);
    nextButton->setIcon(QIcon(":/Images/Next"));
    nextButton->setFixedSize(25, 25);
    nextButton->setStyleSheet(ButtonStyle::primary());

    controlsLayout->addStretch();
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addStretch();

    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);
    progressSlider->setStyleSheet(SliderStyle::primary());

    QHBoxLayout* volumeLayout = new QHBoxLayout;
    volumeLayout->setSpacing(4);

    volumeButton = new QPushButton(this);
    volumeButton->setFixedSize(20, 20);
    volumeButton->setIcon(QIcon(":/Images/Volume"));
    volumeButton->setStyleSheet(ButtonStyle::primary());

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setFixedSize(80, 20);
    volumeSlider->setStyleSheet(SliderStyle::primary());

    volumeLayout->addStretch();
    volumeLayout->addWidget(volumeButton);
    volumeLayout->addWidget(volumeSlider);

    contentLayout->addLayout(topLayout);
    contentLayout->addWidget(mediaDisplayWidget);
    contentLayout->addWidget(songLabel);
    contentLayout->addLayout(controlsLayout);
    contentLayout->addWidget(progressSlider);
    contentLayout->addLayout(volumeLayout);

    rootLayout->addWidget(contentWidget);

    updateDisplayMode();
}

void MiniPlayer::setupConnections()
{
    connect(playPauseButton, &QPushButton::clicked, this, &MiniPlayer::onPlayPauseClicked);
    connect(nextButton, &QPushButton::clicked, this, &MiniPlayer::onNextClicked);
    connect(prevButton, &QPushButton::clicked, this, &MiniPlayer::onPrevClicked);
    connect(progressSlider, &QSlider::sliderMoved, this, &MiniPlayer::onProgressSliderMoved);
    connect(volumeSlider, &QSlider::valueChanged, this, &MiniPlayer::onVolumeSliderChanged);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(toggleVideoButton, &QPushButton::clicked, this, &MiniPlayer::onToggleVideoMode);
}

void MiniPlayer::setPlayerData(QMediaPlayer* player, QAudioOutput* audioOutput)
{
    playerRef = player;
    audioOutputRef = audioOutput;

    if (playerRef && videoWidget)
    {
        playerRef->setVideoOutput(videoWidget);
    }
}

void MiniPlayer::updateTrackInfo(const Track& track)
{
    this->track = track;
    songLabel->setText(track.getTitle());

    if (!track.getThumbnailUrl().isEmpty())
    {
        loadThumbnail(track.getThumbnailUrl());
    }
    else
    {
        QPixmap musicNoteIcon(":/Images/MusicNote");

        if (!musicNoteIcon.isNull()) {
            qreal scaleFactor = 0.5;
            QPixmap scaledIcon = musicNoteIcon.scaled(
                musicNoteIcon.size() * scaleFactor,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );

            QPixmap centeredPixmap(thumbnailLabel->size());
            centeredPixmap.fill(Qt::transparent);

            QPainter painter(&centeredPixmap);
            painter.setRenderHint(QPainter::Antialiasing, true);

            int x = (centeredPixmap.width() - scaledIcon.width()) / 2;
            int y = (centeredPixmap.height() - scaledIcon.height()) / 2;

            painter.drawPixmap(x, y, scaledIcon);

            thumbnailLabel->setPixmap(centeredPixmap);
        }

        thumbnailLabel->setAlignment(Qt::AlignCenter);
        thumbnailLabel->setText("");
    }
}

void MiniPlayer::loadThumbnail(const QString& thumbnailUrl)
{
    if (thumbnailUrl.isEmpty())
        return;

    QNetworkRequest request(thumbnailUrl);
    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if (reply->error() == QNetworkReply::NoError)
                {
                    QByteArray imageData = reply->readAll();
                    QPixmap pixmap;

                    if (pixmap.loadFromData(imageData))
                    {
                        QSize thumbnailSize = thumbnailLabel->size();
                        QPixmap roundedPixmap(thumbnailSize);
                        roundedPixmap.fill(Qt::transparent);

                        QPainter painter(&roundedPixmap);
                        painter.setRenderHint(QPainter::Antialiasing);

                        QPainterPath path;
                        path.addRoundedRect(0, 0, thumbnailSize.width(), thumbnailSize.height(), 8, 8);
                        painter.setClipPath(path);

                        QPixmap scaledPixmap =
                            pixmap.scaled(thumbnailSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                        painter.drawPixmap(0, 0, scaledPixmap);

                        thumbnailLabel->setPixmap(roundedPixmap);
                        thumbnailLabel->setText("");
                    }
                }
                reply->deleteLater();
            });
}

void MiniPlayer::setVideoMode(bool enabled)
{
    isVideoMode = enabled;
    updateDisplayMode();
}

void MiniPlayer::updateDisplayMode()
{
    if (isVideoMode)
    {
        thumbnailLabel->hide();
        videoWidget->show();
        toggleVideoButton->setIcon(QIcon(":/Images/MusicVideo"));
        toggleVideoButton->setToolTip(tr("Switch to Thumbnail"));

        if (playerRef)
        {
            playerRef->setVideoOutput(videoWidget);
        }
    }
    else
    {
        videoWidget->hide();
        thumbnailLabel->show();
        toggleVideoButton->setIcon(QIcon(":/Images/ImageMode"));
        toggleVideoButton->setToolTip(tr("Switch to Video"));

        if (playerRef)
        {
            playerRef->setVideoOutput(nullptr);
        }
    }
}

void MiniPlayer::onToggleVideoMode()
{
    isVideoMode = !isVideoMode;
    updateDisplayMode();
}

void MiniPlayer::updateProgress(qint64 position, qint64 duration)
{
    if (!progressSlider->isSliderDown() && duration > 0)
    {
        progressSlider->setMaximum(static_cast<int>(duration));
        progressSlider->setValue(static_cast<int>(position));
    }
}

void MiniPlayer::updatePlayPauseButton(bool isPlaying)
{
    if (isPlaying)
    {
        playPauseButton->setIcon(QIcon(":/Images/Pause"));
    }
    else
    {
        playPauseButton->setIcon(QIcon(":/Images/Play"));
    }
}

void MiniPlayer::updateVolumeSlider(int volume)
{
    volumeSlider->setValue(volume);
}

void MiniPlayer::onPlayPauseClicked()
{
    emit playPauseClicked();
}

void MiniPlayer::onNextClicked()
{
    emit nextClicked();
}

void MiniPlayer::onPrevClicked()
{
    emit prevClicked();
}

void MiniPlayer::onProgressSliderMoved(int position)
{
    emit progressChanged(position);
}

void MiniPlayer::onVolumeSliderChanged(int volume)
{
    emit volumeChanged(volume);
}

void MiniPlayer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragStartPosition = event->globalPosition().toPoint();
        windowStartPosition = pos();
    }
    QWidget::mousePressEvent(event);
}

void MiniPlayer::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging && (event->buttons() & Qt::LeftButton))
    {
        QPoint delta = event->globalPosition().toPoint() - dragStartPosition;
        move(windowStartPosition + delta);
    }
    QWidget::mouseMoveEvent(event);
}

void MiniPlayer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

void MiniPlayer::closeEvent(QCloseEvent* event)
{
    emit miniPlayerClosed();
    event->accept();
}
