#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QVariant>

class DatabaseManager : public QObject
{
    Q_OBJECT

  public:
    static DatabaseManager& instance();

    bool open();
    void close();
    bool isOpen() const;
    QString databasePath() const;

    // Core query execution
    QSqlQuery executeQuery(const QString& queryStr, const QMap<QString, QVariant>& bindings = {});
    bool execute(const QString& queryStr, const QMap<QString, QVariant>& bindings = {});

    // Transaction support
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    // Utility methods
    bool tableExists(const QString& tableName);
    QSqlDatabase& database() { return m_db; }

  private:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    QString m_databasePath;
};

#endif // DATABASEMANAGER_H
