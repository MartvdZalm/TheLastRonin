#include "ORM.h"
#include <QDebug>

ORM::ORM(QObject* parent) : QObject(parent) {}

ORM& ORM::instance()
{
    static ORM instance;
    return instance;
}

bool ORM::initialize()
{
    DatabaseManager& db = DatabaseManager::instance();
    if (!db.open())
    {
        return false;
    }

    return createTables();
}

bool ORM::createTables()
{
    bool success = true;

    for (auto it = m_registeredModels.begin(); it != m_registeredModels.end(); ++it)
    {
        const ModelInfo& info = it.value();
        if (!createTable(info.tableName, info.schema))
        {
            success = false;
        }
    }

    if (success)
    {
        emit tablesCreated();
    }

    return success;
}

bool ORM::createTable(const QString& tableName, const QStringList& schema)
{
    if (tableExists(tableName))
    {
        qDebug() << "Table" << tableName << "already exists";
        return true;
    }

    QString sql = QString("CREATE TABLE %1 (%2)").arg(tableName).arg(schema.join(", "));

    DatabaseManager& db = DatabaseManager::instance();
    bool result = db.execute(sql);

    if (result)
    {
        qInfo() << "Created table:" << tableName;
    }
    else
    {
        qCritical() << "Failed to create table:" << tableName;
    }

    return result;
}

bool ORM::tableExists(const QString& tableName)
{
    return DatabaseManager::instance().tableExists(tableName);
}

QStringList ORM::getRegisteredTables() const
{
    QStringList tables;
    for (auto it = m_registeredModels.begin(); it != m_registeredModels.end(); ++it)
    {
        tables << it.value().tableName;
    }
    return tables;
}

std::unique_ptr<Model> ORM::createModel(const QString& tableName)
{
    if (m_tableToType.contains(tableName))
    {
        QString typeName = m_tableToType[tableName];
        if (m_registeredModels.contains(typeName))
        {
            return m_registeredModels[typeName].factory();
        }
    }
    return nullptr;
}
