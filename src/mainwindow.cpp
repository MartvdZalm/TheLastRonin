#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollArea>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playlistManager.setPlaylistGrid(ui->playlistGridLayout, this);
    playlistManager.refreshGrid();

    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, [this]() {
        playlistManager.openAddPlaylistDialog();
    });

    connect(ui->searchInput, &QLineEdit::textChanged, this, [this]() {
        playlistManager.search(ui->searchInput->text().trimmed());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
