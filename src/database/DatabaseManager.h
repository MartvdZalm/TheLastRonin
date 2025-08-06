#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QMutex>
#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT

  public:
    static DatabaseManager& instance();

    bool initialize();
    bool isInitialized() const
    {
        return m_initialized;
    }
    QSqlDatabase& database()
    {
        return m_db;
    }

    bool execute(const QString& query, const QVariantMap& params = QVariantMap());

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

  private:
    DatabaseManager() = default;
    ~DatabaseManager();

    bool createTables();
    bool applyPragmas();
    bool verifyDatabase();

    QSqlDatabase m_db;
    QMutex m_mutex;
    bool m_initialized = false;
    static QMutex s_instanceMutex;
};

#endif // DATABASEMANAGER_H
