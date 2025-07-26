#include "MiniPlayerWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QScreen>

MiniPlayerWindow::MiniPlayerWindow(QWidget* parent)
    : QWidget(parent), playerRef(nullptr), audioOutputRef(nullptr), isDragging(false)
{
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setMaximumWidth(250);
    this->adjustSize();

    this->setupUI();
    this->setStyle();

    QScreen* screen = QApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    this->move(screenRect.width() - width() - 20, screenRect.height() - height() - 50);
}

MiniPlayerWindow::~MiniPlayerWindow() {}

void MiniPlayerWindow::setupUI()
{
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");

    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(12, 12, 12, 12);
    contentLayout->setSpacing(10);

    songLabel = new QLabel("No song playing", this);
    songLabel->setAlignment(Qt::AlignCenter);
    songLabel->setWordWrap(true);
    songLabel->setMinimumHeight(40);
    songLabel->setText(track.title);
    songLabel->setObjectName("songLabel");

    QHBoxLayout* controlsLayout = new QHBoxLayout;
    controlsLayout->setSpacing(6);

    prevButton = new QPushButton(this);
    playPauseButton = new QPushButton(this);
    nextButton = new QPushButton(this);
    styleButton(prevButton, ":/Images/Prev");
    styleButton(playPauseButton, ":/Images/Pause");
    styleButton(nextButton, ":/Images/Next");

    controlsLayout->addStretch();
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addStretch();

    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);

    QHBoxLayout* volumeLayout = new QHBoxLayout;
    volumeLayout->setSpacing(4);
    volumeLayout->setAlignment(Qt::AlignRight);

    volumeButton = new QPushButton(this);
    volumeButton->setFixedSize(20, 20);
    this->styleButton(volumeButton, ":/Images/Volume");

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    volumeSlider->setFixedSize(80, 20);

    volumeLayout->addStretch();
    volumeLayout->addWidget(volumeButton);
    volumeLayout->addWidget(volumeSlider);

    QHBoxLayout* windowLayout = new QHBoxLayout;
    windowLayout->setAlignment(Qt::AlignRight);
    closeButton = new QPushButton(this);
    closeButton->setFixedSize(20, 20);
    this->styleButton(closeButton, ":/Images/Close");
    windowLayout->addWidget(closeButton);

    contentLayout->addLayout(windowLayout);
    contentLayout->addWidget(songLabel);
    contentLayout->addLayout(controlsLayout);
    contentLayout->addWidget(progressSlider);
    contentLayout->addLayout(volumeLayout);
    contentLayout->addStretch();

    rootLayout->addWidget(contentWidget);

    connect(playPauseButton, &QPushButton::clicked, this, &MiniPlayerWindow::onPlayPauseClicked);
    connect(nextButton, &QPushButton::clicked, this, &MiniPlayerWindow::onNextClicked);
    connect(prevButton, &QPushButton::clicked, this, &MiniPlayerWindow::onPrevClicked);
    connect(progressSlider, &QSlider::sliderMoved, this, &MiniPlayerWindow::onProgressSliderMoved);
    connect(volumeSlider, &QSlider::valueChanged, this, &MiniPlayerWindow::onVolumeSliderChanged);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}

void MiniPlayerWindow::setStyle()
{
    setStyleSheet(R"(
        #contentWidget {
            background-color: rgba(42, 42, 42, 230);
            border-radius: 12px;
        }

        QPushButton {
            background-color: rgba(74, 144, 226, 180);
            color: white;
            border: none;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: rgba(90, 160, 242, 200);
        }

        QPushButton:pressed {
            background-color: rgba(58, 128, 210, 180);
        }

        QLabel {
            color: #e0e0e0;
            font-size: 11px;
            font-weight: bold;
        }

        QSlider {
            height: 4px;
        }

        QSlider::groove:horizontal {
            background: rgba(56, 56, 56, 180);
            height: 4px;
            border-radius: 2px;
        }

        QSlider::handle:horizontal {
            background: #4a90e2;
            width: 8px;
            height: 8px;
            margin: -2px 0;
            border-radius: 4px;
        }

        QSlider::sub-page:horizontal {
            background: #4a90e2;
            border-radius: 2px;
        }

        #songLabel {
            font-size: 16px;
        }
    )");
}

void MiniPlayerWindow::styleButton(QPushButton* button, const QString& iconPath)
{
    if (!iconPath.isEmpty())
    {
        button->setIcon(QIcon(iconPath));
    }
    button->setObjectName("iconButton");
}

void MiniPlayerWindow::setPlayerData(QMediaPlayer* player, QAudioOutput* audioOutput)
{
    playerRef = player;
    audioOutputRef = audioOutput;
}

void MiniPlayerWindow::updateTrackInfo(const Track& track)
{
    this->track = track;
    songLabel->setText(track.title);
}

void MiniPlayerWindow::updateProgress(qint64 position, qint64 duration)
{
    if (!progressSlider->isSliderDown() && duration > 0)
    {
        progressSlider->setMaximum(static_cast<int>(duration));
        progressSlider->setValue(static_cast<int>(position));
    }
}

void MiniPlayerWindow::updatePlayPauseButton(bool isPlaying)
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

void MiniPlayerWindow::updateVolumeSlider(int volume)
{
    volumeSlider->setValue(volume);
}

void MiniPlayerWindow::onPlayPauseClicked()
{
    emit playPauseClicked();
}

void MiniPlayerWindow::onNextClicked()
{
    emit nextClicked();
}

void MiniPlayerWindow::onPrevClicked()
{
    emit prevClicked();
}

void MiniPlayerWindow::onProgressSliderMoved(int position)
{
    emit progressChanged(position);
}

void MiniPlayerWindow::onVolumeSliderChanged(int volume)
{
    emit volumeChanged(volume);
}

void MiniPlayerWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragStartPosition = event->globalPosition().toPoint();
        windowStartPosition = pos();
    }
    QWidget::mousePressEvent(event);
}

void MiniPlayerWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging && (event->buttons() & Qt::LeftButton))
    {
        QPoint delta = event->globalPosition().toPoint() - dragStartPosition;
        move(windowStartPosition + delta);
    }
    QWidget::mouseMoveEvent(event);
}

void MiniPlayerWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

void MiniPlayerWindow::closeEvent(QCloseEvent* event)
{
    emit miniPlayerClosed();
    event->accept();
}
