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

  private:
    QStackedWidget* stackedWidget;

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void goBack();

  private:
    void showHomePage();
    void showPlaylistPage(const Playlist& playlist);
    void showSettingsPage();
    void setPage(BaseWindow* newPage, bool addToHistory = true);
};

#endif // MAINWINDOW_H
