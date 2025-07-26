#include "PlaylistWindow.h"
#include "../components/dialog/AddTrackDialog.h"
#include "../components/dialog/PlaylistDialog.h"
#include "../components/shared/NavigationBar.h"
#include "../events/AppEvents.h"
#include "../styles/ButtonStyle.h"
#include "MainWindow.h"
#include <QListWidgetItem>
#include <QPixmap>
#include <QTimer>

PlaylistWindow::PlaylistWindow(const Playlist& playlist, QWidget* parent) : BaseWindow(parent), playlistData(playlist)
{
    this->setupUI();
    this->setupConnections();
}

PlaylistWindow::~PlaylistWindow() {}

void PlaylistWindow::setupUI()
{
    this->setWindowTitle(playlistData.name);
    this->showMaximized();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    NavigationBar* navBar = new NavigationBar(this);

    connect(navBar, &NavigationBar::backClicked, this,
            [this]()
            {
                if (auto mainWindow = qobject_cast<MainWindow*>(window()))
                {
                    mainWindow->goBack();
                }
            });

    mainLayout->addWidget(navBar);

    addTrackBtn = new QPushButton("Add Track");
    addTrackBtn->setStyleSheet(ButtonStyle::styleSheet());

    editPlaylistBtn = new QPushButton("Edit Playlist");
    editPlaylistBtn->setStyleSheet(ButtonStyle::styleSheet());

    removePlaylistBtn = new QPushButton("Delete Playlist");
    removePlaylistBtn->setStyleSheet(ButtonStyle::styleSheet());

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

    playbackBarWidget = PlaybackBar::instance();
    playbackBarWidget->updatePlaylist(playlistData);
}

void PlaylistWindow::setupConnections()
{
    connect(addTrackBtn, &QPushButton::clicked, this,
            [=]()
            {
                AddTrackDialog dialog(this);
                if (dialog.exec() == QDialog::Accepted)
                {
                    Track newTrack = dialog.getTrack();

                    trackDAO.insertTrack(playlistData.id, newTrack);

                    trackList->addTrack(newTrack);
                    playlistData.tracks.append(newTrack);
                }
            });

    connect(editPlaylistBtn, &QPushButton::clicked, this,
            [=]()
            {
                auto result = this->showEditPlaylistDialog();
                if (result.has_value())
                {
                    refreshMetadata(result.value());
                    AppEvents::instance().notifyPlaylistChanged();
                }
            });

    connect(removePlaylistBtn, &QPushButton::clicked, this,
            [=]()
            {
                playlistDAO.deletePlaylist(playlistData.id);
                AppEvents::instance().notifyPlaylistChanged();
            });

    connect(trackList, &QListWidget::itemClicked, this,
            [=](QListWidgetItem* item)
            {
                int index = trackList->row(item);
                playTrackAtIndex(index);
            });

    connect(playbackBarWidget, &PlaybackBar::nextClicked, this,
            [this]()
            {
                if (currentTrackIndex < playlistData.tracks.size() - 1)
                {
                    currentTrackIndex++;
                    playTrackAtIndex(currentTrackIndex);
                }
            });

    connect(playbackBarWidget, &PlaybackBar::prevClicked, this,
            [this]()
            {
                if (currentTrackIndex > 0)
                {
                    currentTrackIndex--;
                    playTrackAtIndex(currentTrackIndex);
                }
            });
}

void PlaylistWindow::setupEvents() {}

std::optional<Playlist> PlaylistWindow::showEditPlaylistDialog()
{
    PlaylistDialog dialog(playlistData, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Playlist playlist{
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

void PlaylistWindow::playTrackAtIndex(int index)
{
    if (index >= 0 && index < playlistData.tracks.size())
    {
        currentTrackIndex = index;

        playbackBarWidget->playTrackAtIndex(index);

        trackList->setCurrentRow(index);
    }
}

void PlaylistWindow::refreshMetadata(const Playlist& updatedPlaylist)
{
    playlistData.name = updatedPlaylist.name;
    playlistData.description = updatedPlaylist.description;
    playlistData.coverImagePath = updatedPlaylist.coverImagePath;

    setWindowTitle(playlistData.name);

    if (detailsWidget)
    {
        detailsWidget->updateDetails(playlistData);
    }

    if (coverImageWidget)
    {
        coverImageWidget->setImage(playlistData.coverImagePath);
    }
}

void PlaylistWindow::styleButton(QPushButton* button, const QString& iconPath)
{
    if (!iconPath.isEmpty())
    {
        button->setIcon(QIcon(iconPath));
    }
    button->setObjectName("iconButton");
}
