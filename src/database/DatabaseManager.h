#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    static DatabaseManager& instance();

    bool openDatabase();
    void closeDatabase();
    bool initSchema();
    bool deleteUserData();
    bool executeQuery(const QString& query, const QMap<QString, QVariant>& bindings = {});

private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
