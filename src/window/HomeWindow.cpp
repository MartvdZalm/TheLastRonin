#include "HomeWindow.h"
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
}

void HomeWindow::setupUI()
{
    this->setWindowTitle("Home");
    this->resize(1000, 700);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Top Bar (Search + Add Playlist)
    QHBoxLayout* topBar = new QHBoxLayout;
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Search playlists...");
    searchInput->setClearButtonEnabled(true);

    addPlaylistBtn = new QPushButton("Add Playlist", this);
    topBar->addWidget(searchInput, 3);
    topBar->addWidget(addPlaylistBtn, 1);

    mainLayout->addLayout(topBar);

    // Playlist Grid inside ScrollArea
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollContent = new QWidget(scrollArea);
    playlistGridLayout = new QGridLayout(scrollContent);
    playlistGridLayout->setSpacing(15);
    scrollContent->setLayout(playlistGridLayout);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    playlistManager.setPlaylistGrid(playlistGridLayout, this);
    playlistManager.refreshGrid();
}

void HomeWindow::setupConnections()
{
    connect(addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        playlistManager.openAddPlaylistDialog();
    });

    connect(searchInput, &QLineEdit::textChanged, this, [this]() {
        playlistManager.search(searchInput->text().trimmed());
    });
}

void HomeWindow::reload()
{
    playlistManager.refreshGrid();
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
