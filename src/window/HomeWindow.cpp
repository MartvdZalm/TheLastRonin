#include "HomeWindow.h"
#include "../components/dialog/PlaylistDialog.h"
#include "../events/AppEvents.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>
#include "../components/shared/NavigationBar.h"
#include "MainWindow.h"

HomeWindow::HomeWindow(QWidget* parent)
    : BaseWindow(parent)
{
    setStyle();
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

    connect(navBar, &NavigationBar::backClicked, this, [this]() {
        if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
            mainWindow->goBack();
        }
    });

    mainLayout->addWidget(navBar);

    QHBoxLayout* topBar = new QHBoxLayout;
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Search playlists...");
    searchInput->setClearButtonEnabled(true);
    topBar->addWidget(searchInput);
    mainLayout->addLayout(topBar);

    QHBoxLayout* sortFilterBar = new QHBoxLayout;
    deleteDataBtn = new QPushButton("Delete Data", this);
    sortFilterBar->addWidget(deleteDataBtn);
    addPlaylistBtn = new QPushButton("Add Playlist", this);
    sortFilterBar->addWidget(addPlaylistBtn);
    importPlaylistBtn = new QPushButton("Import Playlist", this);
    sortFilterBar->addWidget(importPlaylistBtn);

    sortComboBox = new QComboBox(this);
    sortComboBox->addItem("Choose Filter");
    sortComboBox->addItem("Sort by Name");
    sortComboBox->addItem("Sort by Creation Date");
    sortComboBox->addItem("Sort by Recently Played");
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

    updatePlaylistGrid(playlistDAO.getAllPlaylists());
}

void HomeWindow::setupConnections()
{
    connect(deleteDataBtn, &QPushButton::clicked, this, [this]() {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Confirm Deletion",
                                      "Are you sure you want to delete all your data? This cannot be undone.",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            DatabaseManager::instance().deleteUserData();
        }
    });

    connect(addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        this->showPlaylistDialog();
    });

    connect(importPlaylistBtn, &QPushButton::clicked, this, [this]() {
        this->importPlaylistFromFolder();
    });

    connect(searchInput, &QLineEdit::textChanged, this, [this]() {
        this->searchPlaylists(searchInput->text().trimmed());
    });

    connect(sortComboBox, &QComboBox::currentTextChanged, this, &HomeWindow::onSortChanged);
}

void HomeWindow::setStyle()
{
    this->setStyleSheet(R"(
        QLineEdit {
            padding: 8px;
            border: 1px solid #444;
            border-radius: 5px;
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
    )");
}

void HomeWindow::setupEvents()
{
    connect(&AppEvents::instance(), &AppEvents::playlistChanged, this, [this]() {
        updatePlaylistGrid(playlistDAO.getAllPlaylists());
    });
}

void HomeWindow::showPlaylistDialog()
{
    PlaylistDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Playlist playlist {
            .name = dialog.getName(),
            .description = dialog.getDescription(),
            .coverImagePath = dialog.getCoverImagePath(),
        };

        playlist.id = playlistDAO.insertPlaylist(playlist);
        playlistGrid->addPlaylist(playlist);
    }
}

void HomeWindow::importPlaylistFromFolder()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Playlist Folder");

    if (folderPath.isEmpty()) {
        return;
    }

    QDir dir(folderPath);
    QString playlistName = dir.dirName();

    QString coverImagePath;
    QStringList imageFiles = dir.entryList({ "*.png", "*.jpg", "*.jpeg" }, QDir::Files);
    if (!imageFiles.isEmpty()) {
        coverImagePath = dir.filePath(imageFiles.first());
    }

    QStringList audioFiles;
    QDirIterator it(folderPath, { "*.mp3", "*.wav", "*.flac" }, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        audioFiles << it.next();
    }

    if (audioFiles.isEmpty()) {
        QMessageBox::warning(this, "No Tracks Found", "No audio files were found in the selected folder.");
        return;
    }

    Playlist playlist {
        .name = playlistName,
        .coverImagePath = coverImagePath
    };
    playlist.id = playlistDAO.insertPlaylist(playlist);

    for (const QString& filePath : audioFiles) {
        QFileInfo fileInfo(filePath);
        Track track {
            .title = fileInfo.baseName(),
            .filePath = filePath,
        };

        trackDAO.insertTrack(playlist.id, track);
    }

    playlistGrid->addPlaylist(playlist);
    AppEvents::instance().notifyPlaylistChanged();
}

void HomeWindow::searchPlaylists(const QString& query)
{
    updatePlaylistGrid(playlistDAO.searchPlaylists(query));
}

void HomeWindow::onSortChanged(const QString& sortBy)
{
    QList<Playlist> playlists = playlistDAO.getAllPlaylists();

    if (sortBy == "Sort by Name") {
        std::sort(playlists.begin(), playlists.end(), [](const Playlist& a, const Playlist& b) {
            return a.name < b.name;
        });
    } else if (sortBy == "Sort by Creation Date") {
        std::sort(playlists.begin(), playlists.end(), [](const Playlist& a, const Playlist& b) {
            return a.createdAt < b.createdAt;
        });
    } else if (sortBy == "Sort by Recently Played") {
        // std::sort(playlists.begin(), playlists.end(), [](const Playlist& a, const Playlist& b) {
        //     return a.lastPlayed > b.lastPlayed;
        // });
    }

    updatePlaylistGrid(playlists);
}

void HomeWindow::updatePlaylistGrid(const QList<Playlist>& playlists)
{
    playlistGrid->clearGrid();
    for (const Playlist& playlist : playlists) {
        playlistGrid->addPlaylist(playlist);
    }
}

void HomeWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    playlistGrid->updateColumns();
}
