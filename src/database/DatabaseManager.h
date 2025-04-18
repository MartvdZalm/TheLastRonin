#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include "../dialog/Album.h"

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDatabase();
    bool initSchema();

    int insertAlbum(const QString& title, const QString& artist, const QString& coverPath, const QString& releaseDate);

    QVector<Album> getAllAlbums();

private:
    DatabaseManager();
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
