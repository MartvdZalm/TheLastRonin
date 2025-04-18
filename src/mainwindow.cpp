#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog/AddAlbumDialog.h"
#include "dialog/AlbumCard.h"

#include <QScrollArea>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(&DatabaseManager::instance())
{
    ui->setupUi(this);

    if (db->openDatabase()) {
        db->initSchema();
    }

    connect(ui->addAlbumBtn, &QPushButton::clicked, this, &MainWindow::openAddAlbumDialog);

    albumGrid = ui->albumGridLayout;
    albumGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QVector<Album> albums = db->getAllAlbums();
    for (const Album& album : albums) {
        addAlbumToGrid(album);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAddAlbumDialog()
{
    AddAlbumDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Album newAlbum {
            .title = dialog.getTitle(),
            .artist = dialog.getArtist(),
            .coverPath = dialog.getCoverPath()
        };

        int id = db->insertAlbum(newAlbum.title, newAlbum.artist, newAlbum.coverPath, newAlbum.releaseDate);
        newAlbum.id = id;

        addAlbumToGrid(newAlbum);
    }
}

void MainWindow::addAlbumToGrid(const Album& album)
{
    AlbumCard* card = new AlbumCard(album);
    albumGrid->addWidget(card, currentRow, currentCol);

    currentCol++;
    if (currentCol >= maxCols) {
        currentCol = 0;
        currentRow++;
    }
}
