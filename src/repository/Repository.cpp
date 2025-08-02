#include "Repository.h"
#include <QDateTime>
#include <QSqlTableModel>

template <typename T>
bool Repository<T>::save(T* model)
{
    if (!model || !model->isValid())
    {
        return false;
    }

    applyTimestamps(model, model->id() > 0);

    if (model->id() <= 0)
    {
        return insert(model);
    }
    else
    {
        return update(model);
    }
}

template <typename T>
bool Repository<T>::insert(T* model)
{
    QVariantMap data = model->toVariantMap();
    data.remove("id"); // Remove ID for auto-increment

    QStringList columns = data.keys();
    QStringList placeholders;
    QMap<QString, QVariant> bindings;

    for (const QString& column : columns)
    {
        placeholders << ":" + column;
        bindings[column] = data[column];
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                      .arg(model->getTableName())
                      .arg(columns.join(", "))
                      .arg(placeholders.join(", "));

    QSqlQuery query = databaseManager->executeQuery(sql, bindings);
    if (query.isActive())
    {
        model->setId(query.lastInsertId().toInt());
        return true;
    }

    return false;
}

template <typename T>
std::unique_ptr<T> Repository<T>::find(int id)
{
    T model;
    QString sql = QString("SELECT * FROM %1 WHERE id = :id").arg(model.getTableName());

    QSqlQuery query = databaseManager->executeQuery(sql, {{"id", id}});
    if (query.next())
    {
        return createFromRecord(query.record());
    }

    return nullptr;
}

template <typename T>
QList<std::unique_ptr<T>> Repository<T>::findAll()
{
    QList<std::unique_ptr<T>> results;

    T model;
    QString sql = QString("SELECT * FROM %1").arg(model.getTableName());

    QSqlQuery query = databaseManager->executeQuery(sql);
    if (query.isActive())
    {
        while (query.next())
        {
            QSqlRecord record = query.record();
            T item = std::make_unique<T>();
            item->deserialize(record);
            results.append(std::move(item));
        }
    }

    return results;
}