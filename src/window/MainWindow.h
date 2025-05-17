#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include "../model/Playlist.h"
#include "BaseWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void showHomePage();
    void showPlaylistPage(const Playlist& playlist);

    void goBack();
    void goForward();

private:
    BaseWindow* currentPage = nullptr;
    QStack<BaseWindow*> backStack;
    QStack<BaseWindow*> forwardStack;

    void setPage(BaseWindow* newPage, bool addToHistory = true);
};

#endif // MAINWINDOW_H

