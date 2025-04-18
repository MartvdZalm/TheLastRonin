#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include "../album/Album.h"

class DatabaseManager
{
public:
    DatabaseManager();

    static DatabaseManager& instance();

    bool openDatabase();
    bool initSchema();

    int insertAlbum(const QString& title, const QString& artist, const QString& coverPath, const QString& releaseDate);

    QVector<Album> getAllAlbums();
    QVector<Album> getAlbumsByGenre(const QString& genre);

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
