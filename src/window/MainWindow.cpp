#include "MainWindow.h"
#include "HomeWindow.h"
#include "PlaylistWindow.h"
#include "../events/AppEvents.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    showHomePage();

    this->setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }
    )");

    connect(&AppEvents::instance(), &AppEvents::navigateToPlaylist, this, &MainWindow::showPlaylistPage);
}

MainWindow::~MainWindow()
{
    delete currentPage;
    while (!backStack.isEmpty()) {
        delete backStack.pop();
    }
    while (!forwardStack.isEmpty()) {
        delete forwardStack.pop();
    }
}

void MainWindow::setPage(BaseWindow* newPage, bool addToHistory)
{
    if (currentPage && addToHistory) {
        backStack.push(currentPage);
        currentPage->hide();
    } else if (currentPage) {
        delete currentPage;
    }

    forwardStack.clear();

    currentPage = newPage;
    setCentralWidget(currentPage);
    currentPage->show();
}

void MainWindow::showHomePage()
{
    setPage(new HomeWindow(this));

}

void MainWindow::showPlaylistPage(const Playlist& playlist)
{
    setPage(new PlaylistWindow(playlist, this));
}

void MainWindow::goBack()
{
    if (backStack.isEmpty()) {
        return;
    }

    forwardStack.push(currentPage);
    BaseWindow* previous = backStack.pop();
    setPage(previous, false);
}

void MainWindow::goForward()
{
    if (forwardStack.isEmpty()) {
        return;
    }

    backStack.push(currentPage);
    BaseWindow* next = forwardStack.pop();
    setPage(next, false);
}

