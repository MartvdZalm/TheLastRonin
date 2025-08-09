#include "MainWindow.h"
#include "../events/AppEvents.h"
#include "HomeWindow.h"
#include "LibraryWindow.h"
#include "PlaylistWindow.h"
#include "SettingsWindow.h"
#include "YouTubeMusicWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    QWidget* container = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    stackedWidget = new QStackedWidget(container);

    mainLayout->addWidget(stackedWidget);
    mainLayout->addWidget(PlaybackBar::instance());

    setCentralWidget(container);

    showHomePage();

    this->setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }
    )");

    connect(&AppEvents::instance(), &AppEvents::navigateToHome, this, &MainWindow::showHomePage);
    connect(&AppEvents::instance(), &AppEvents::navigateToPlaylist, this, &MainWindow::showPlaylistPage);
    connect(&AppEvents::instance(), &AppEvents::navigateToSettings, this, &MainWindow::showSettingsPage);
    connect(&AppEvents::instance(), &AppEvents::navigateToLibrary, this, &MainWindow::showLibraryPage);
    connect(&AppEvents::instance(), &AppEvents::navigateToYouTubeMusic, this, &MainWindow::showYouTubeMusicPage);
}

MainWindow::~MainWindow() {}

void MainWindow::setPage(BaseWindow* newPage, bool addToHistory)
{
    if (!newPage)
        return;

    if (stackedWidget->count() > 0)
    {
        if (!addToHistory)
        {
            QWidget* oldPage = stackedWidget->currentWidget();
            stackedWidget->removeWidget(oldPage);
            oldPage->deleteLater();
        }
    }

    stackedWidget->addWidget(newPage);
    stackedWidget->setCurrentWidget(newPage);
}

void MainWindow::showHomePage()
{
    setPage(new HomeWindow(this), false);
}

void MainWindow::showPlaylistPage(const Playlist& playlist)
{
    setPage(new PlaylistWindow(playlist, this));
}

void MainWindow::showSettingsPage()
{
    setPage(new SettingsWindow(this), false);
}

void MainWindow::showLibraryPage()
{
    setPage(new LibraryWindow(this), false);
}

void MainWindow::showYouTubeMusicPage()
{
    setPage(new YouTubeMusicWindow(this), false);
}

void MainWindow::goBack()
{
    if (stackedWidget->count() > 1)
    {
        QWidget* oldPage = stackedWidget->currentWidget();
        stackedWidget->setCurrentIndex(stackedWidget->currentIndex() - 1);
        stackedWidget->removeWidget(oldPage);
        oldPage->deleteLater();
    }
}

void MainWindow::goNext() {}