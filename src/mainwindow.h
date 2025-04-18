#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "album/AlbumManager.h"
#include "playlist/PlaylistManager.h"
#include "database/DatabaseManager.h"
#include <QMainWindow>
#include <QGridLayout>


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void search();

private:
    Ui::MainWindow *ui;
    DatabaseManager* db;
    AlbumManager* albumManager;
    PlaylistManager* playlistManager;
};
#endif // MAINWINDOW_H
