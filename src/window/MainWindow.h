#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../model/Playlist.h"
#include "BaseWindow.h"
#include <QMainWindow>
#include <QStack>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void goBack();
    void goNext();

  private:
    void showHomePage();
    void showPlaylistPage(const Playlist& playlist);
    void showSettingsPage();
    void showLibraryPage();
    void showYouTubeMusicPage();
    void setPage(BaseWindow* newPage, bool addToHistory = true);

    QStackedWidget* stackedWidget;
};

#endif // MAINWINDOW_H
