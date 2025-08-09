#include "HomeWindow.h"

#include "../components/dialog/AddPlaylistDialog.h"
#include "../components/shared/NavigationBar.h"
#include "../components/shared/PlaybackBar.h"
#include "../database/Container.h"
#include "../database/DatabaseManager.h"
#include "../events/AppEvents.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ComboBoxStyle.h"
#include "../styles/InputStyle.h"
#include "MainWindow.h"
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>

HomeWindow::HomeWindow(QWidget* parent) : BaseWindow(parent)
{
    setupUI();
    setupConnections();
    setupEvents();
}

void HomeWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    NavigationBar* navBar = new NavigationBar(this);
    mainLayout->addWidget(navBar);

    QHBoxLayout* topBar = new QHBoxLayout;
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText(tr("Search playlists..."));
    searchInput->setClearButtonEnabled(true);
    searchInput->setStyleSheet(InputStyle::primary());
    topBar->addWidget(searchInput);
    mainLayout->addLayout(topBar);

    QHBoxLayout* sortFilterBar = new QHBoxLayout;
    deleteDataBtn = new QPushButton(tr("Delete Data"), this);
    deleteDataBtn->setStyleSheet(ButtonStyle::remove());
    sortFilterBar->addWidget(deleteDataBtn);

    addPlaylistBtn = new QPushButton(tr("Add Playlist"), this);
    addPlaylistBtn->setStyleSheet(ButtonStyle::primary());
    sortFilterBar->addWidget(addPlaylistBtn);

    importPlaylistBtn = new QPushButton(tr("Import Playlist"), this);
    importPlaylistBtn->setStyleSheet(ButtonStyle::primary());
    sortFilterBar->addWidget(importPlaylistBtn);

    sortComboBox = new QComboBox(this);
    sortComboBox->addItem(tr("Choose Filter"));
    sortComboBox->addItem(tr("Sort by Name"));
    sortComboBox->addItem(tr("Sort by Creation Date"));
    sortComboBox->addItem(tr("Sort by Recently Played"));
    sortComboBox->setStyleSheet(ComboBoxStyle::primary());
    sortFilterBar->addWidget(sortComboBox);
    sortFilterBar->addStretch();

    mainLayout->addLayout(sortFilterBar);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollContent = new QWidget(scrollArea);
    playlistGridLayout = new QGridLayout(scrollContent);
    playlistGridLayout->setSpacing(15);
    scrollContent->setLayout(playlistGridLayout);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
    playlistGrid = new PlaylistGrid(playlistGridLayout, this);

    updatePlaylistGrid(Container::instance().getPlaylistRepository()->findAll());
}

void HomeWindow::setupConnections()
{
    connect(deleteDataBtn, &QPushButton::clicked, this,
            [this]()
            {
                QMessageBox::StandardButton reply;
                reply =
                    QMessageBox::question(this, tr("Confirm Deletion"),
                                          tr("Are you sure you want to delete all your data? This cannot be undone."),
                                          QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    // DatabaseManager::instance().deleteData();
                }
            });

    connect(addPlaylistBtn, &QPushButton::clicked, this, [this]() { this->showPlaylistDialog(); });

    connect(importPlaylistBtn, &QPushButton::clicked, this, [this]() { this->importPlaylistFromFolder(); });

    connect(searchInput, &QLineEdit::textChanged, this,
            [this]() { this->searchPlaylists(searchInput->text().trimmed()); });

    connect(sortComboBox, &QComboBox::currentTextChanged, this, &HomeWindow::onSortChanged);
}

void HomeWindow::setupEvents()
{
    connect(&AppEvents::instance(), &AppEvents::playlistChanged, this,
            [this]() { updatePlaylistGrid(Container::instance().getPlaylistRepository()->findAll()); });
}

void HomeWindow::showPlaylistDialog()
{
    AddPlaylistDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Playlist playlist;
        playlist.setName(dialog.getName());
        playlist.setDescription(dialog.getDescription());
        playlist.setCoverImagePath(dialog.getCoverImagePath());
        playlist.setCreatedAt(QDateTime::currentDateTime());
        playlist.setUpdatedAt(QDateTime::currentDateTime());

        Container::instance().getPlaylistRepository()->save(playlist);

        playlistGrid->addPlaylist(playlist);
    }
}

void HomeWindow::importPlaylistFromFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Playlist Folder"));

    if (folderPath.isEmpty())
    {
        return;
    }

    QDir dir(folderPath);
    QString playlistName = dir.dirName();

    QString coverImagePath;
    QStringList imageFiles = dir.entryList({"*.png", "*.jpg", "*.jpeg"}, QDir::Files);
    if (!imageFiles.isEmpty())
    {
        coverImagePath = dir.filePath(imageFiles.first());
    }

    QStringList audioFiles;
    QDirIterator it(folderPath, {"*.mp3", "*.wav", "*.flac"}, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        audioFiles << it.next();
    }

    if (audioFiles.isEmpty())
    {
        QMessageBox::warning(this, tr("No Tracks Found"), tr("No audio files were found in the selected folder."));
        return;
    }

    Playlist playlist;
    playlist.setName(playlistName);
    playlist.setCoverImagePath(coverImagePath);
    playlist.setCreatedAt(QDateTime::currentDateTime());
    playlist.setUpdatedAt(QDateTime::currentDateTime());
    auto savedPlaylist = Container::instance().getPlaylistRepository()->save(playlist);

    if (!savedPlaylist)
    {
        qWarning() << "Failed to save playlist.";
        return;
    }

    for (const QString& filePath : audioFiles)
    {
        QFileInfo fileInfo(filePath);

        Track track;
        track.setTitle(fileInfo.baseName());
        track.setFilePath(filePath);
        auto savedTrack = trackService.addTrack(track);

        if (!savedTrack)
        {
            qWarning() << "Failed to save track:" << fileInfo.baseName();
            continue;
        }

        Container::instance().getPlaylistRepository()->addTrackToPlaylist(savedPlaylist->getId(), savedTrack->getId());
    }

    playlistGrid->addPlaylist(savedPlaylist.value());
    AppEvents::instance().notifyPlaylistChanged();
}

void HomeWindow::searchPlaylists(const QString& query)
{
    // updatePlaylistGrid(playlistRepository.searchPlaylists(query));
}

void HomeWindow::onSortChanged(const QString& sortBy)
{
    QList<Playlist> playlists = Container::instance().getPlaylistRepository()->findAll();

    if (sortBy == tr("Sort by Name"))
    {
        std::sort(playlists.begin(), playlists.end(),
                  [](const Playlist& a, const Playlist& b) { return a.getName() < b.getName(); });
    }
    else if (sortBy == tr("Sort by Creation Date"))
    {
        std::sort(playlists.begin(), playlists.end(),
                  [](const Playlist& a, const Playlist& b) { return a.getCreatedAt() < b.getCreatedAt(); });
    }
    else if (sortBy == tr("Sort by Recently Played"))
    {
        // std::sort(playlists.begin(), playlists.end(), [](const Playlist& a, const Playlist& b) {
        //     return a.lastPlayed > b.lastPlayed;
        // });
    }

    updatePlaylistGrid(playlists);
}

void HomeWindow::updatePlaylistGrid(const QList<Playlist>& playlists)
{
    playlistGrid->clearGrid();
    for (const Playlist& playlist : playlists)
    {
        playlistGrid->addPlaylist(playlist);
    }
}

void HomeWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    playlistGrid->updateColumns();
}
