#ifndef ALBUMMANAGER_H
#define ALBUMMANAGER_H

#include "../database/DatabaseManager.h"
#include <QGridLayout>

class AlbumManager : public QObject
{
    Q_OBJECT

public:
    explicit AlbumManager(QObject* parent = nullptr);

    static AlbumManager& instance();

    void setAlbumGrid(QGridLayout* albumGrid);
    void openAddAlbumDialog(QWidget* parent);
    void addAlbumToGrid(const Album& album);
    void applyFilterToGrid(const QString& selectedFilter);
    void clearAlbumGrid();

private slots:
    void openAlbumPage(const Album& album);

private:
    DatabaseManager* db;
    QGridLayout* albumGrid;

    int currentRow = 0;
    int currentCol = 0;
    int maxCols = 4;
};

#endif // ALBUMMANAGER_H
