#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
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

private:
    QStackedWidget* stackedWidget;

    void setPage(BaseWindow* newPage, bool addToHistory = true);
};

#endif // MAINWINDOW_H

