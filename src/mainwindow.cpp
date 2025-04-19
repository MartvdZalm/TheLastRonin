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

    QVector<Playlist> playlis = db->getAllPlaylists();
    for (const Playlist& playlist : playlis) {
        playlistManager->addPlaylistToGrid(playlist);
    }

    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        playlistManager->openAddPlaylistDialog(this);
    });

    // connect(ui->albumFilterBox, &QComboBox::currentTextChanged, this, [this]() {
    //     albumManager->applyFilterToGrid()
    // });
}

MainWindow::~MainWindow()
{
    delete ui;
}
