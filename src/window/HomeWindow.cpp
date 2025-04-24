#include "HomeWindow.h"
#include "../components/dialog/PlaylistDialog.h"
#include "../events/AppEvents.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

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
    this->setWindowTitle("Home");
    this->resize(1000, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QHBoxLayout* topBar = new QHBoxLayout;
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Search playlists...");
    searchInput->setClearButtonEnabled(true);

    addPlaylistBtn = new QPushButton("Add Playlist", this);
    topBar->addWidget(searchInput, 3);
    topBar->addWidget(addPlaylistBtn, 1);
    mainLayout->addLayout(topBar);

    QHBoxLayout* sortFilterBar = new QHBoxLayout;

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
    connect(addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        this->showPlaylistDialog();
    });

    connect(searchInput, &QLineEdit::textChanged, this, [this]() {
        this->searchPlaylists(searchInput->text().trimmed());
    });

    connect(sortComboBox, &QComboBox::currentTextChanged, this, &HomeWindow::onSortChanged);
}

void HomeWindow::setStyle()
{
    this->setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

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
