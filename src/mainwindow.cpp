#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollArea>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(&DatabaseManager::instance())
    , albumManager(&AlbumManager::instance())
{
    ui->setupUi(this);

    albumManager->setAlbumGrid(ui->albumGridLayout);

    QVector<Album> albums = db->getAllAlbums();
    for (const Album& album : albums) {
        albumManager->addAlbumToGrid(album);
    }

    connect(ui->addAlbumBtn, &QPushButton::clicked, this, [this]() {
        albumManager->openAddAlbumDialog(this);
    });

    // connect(ui->albumFilterBox, &QComboBox::currentTextChanged, this, [this]() {
    //     albumManager->applyFilterToGrid()
    // });
}

MainWindow::~MainWindow()
{
    delete ui;
}
