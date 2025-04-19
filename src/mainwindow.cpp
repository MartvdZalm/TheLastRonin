#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollArea>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(&DatabaseManager::instance())
    , albumManager(&AlbumManager::instance())
    , playlistManager(&PlaylistManager::instance())
{
    ui->setupUi(this);

    playlistManager->setPlaylistGrid(ui->playlistGridLayout);

    QVector<Playlist> playlists = db->getAllPlaylists();
    for (const Playlist& playlist : playlists) {
        playlistManager->addPlaylistToGrid(playlist);
    }

    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        playlistManager->openAddPlaylistDialog(this);
    });

    connect(ui->searchBtn, &QPushButton::clicked, this, &MainWindow::search);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::search);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::search()
{
    QString query = ui->searchInput->text().trimmed();

    playlistManager->clearPlaylistGrid();

    QVector<Playlist> results = db->searchPlaylists(query);

    for (const Playlist& playlist : results) {
        playlistManager->addPlaylistToGrid(playlist);
    }
}
