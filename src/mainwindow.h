#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database/DatabaseManager.h"
#include "dialog/Album.h"
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

    void openAddAlbumDialog();
    void addAlbumToGrid(const Album& album);

private:
    Ui::MainWindow *ui;
    QGridLayout* albumGrid;
    DatabaseManager* db;

    int currentRow = 0;
    int currentCol = 0;
    int maxCols = 4;
};
#endif // MAINWINDOW_H
