#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../database/QueryBuilder.h"
#include "../model/Model.h"
#include <QSqlRecord>
#include <QSqlTableModel>
#include <memory>

template <typename T> class Repository : public QObject
{
    static_assert(std::is_base_of_v<Model, T>, "T must inherit from Model");

  public:
    explicit Repository(QObject* parent = nullptr) : QObject(parent), databaseManager(&DatabaseManager::instance()) {}

    bool save(T* model)
    {
        if (!model || !model->isValid())
        {
            return false;
        }

        applyTimestamps(model, model->id() > 0);

        return model->id() <= 0 ? insert(model) : update(model);
    }

    std::unique_ptr<T> find(int id)
    {
        T model;
        auto records = QueryBuilder::table(model.getTableName()).where("id", id).get();

        if (!records.isEmpty())
        {
            auto item = std::make_unique<T>();
            item->deserialize(records.first());
            return item;
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<T>> findAll()
    {
        std::vector<std::unique_ptr<T>> results;
        T model;

        auto records = QueryBuilder::table(model.getTableName()).get();

        for (const auto& record : records)
        {
            auto item = std::make_unique<T>();
            item->deserialize(record);
            results.push_back(std::move(item));
        }

        return results;
    }

  private:
    bool insert(T* model)
    {
        if (!model)
            return false;

        QVariantMap data = model->toVariantMap();
        data.remove("id");

        auto query = QueryBuilder::table(model->getTableName()).insert(data);

        if (!query.isActive())
        {
            return false;
        }

        model->setId(query.lastInsertId().toInt());
        return true;
    }

    bool update(T* model)
    {
        if (!model || model->id() <= 0)
        {
            return false;
        }

        QVariantMap data = model->toVariantMap();
        data.remove("id");

        return QueryBuilder::table(model->getTableName()).where("id", model->id()).update(data);
    }

    void applyTimestamps(T* model, bool isUpdate)
    {
        if (!model->hasTimestamps())
        {
            return;
        }

        QDateTime now = QDateTime::currentDateTime();
        if (isUpdate)
        {
            model->setProperty(model->updatedAtColumn().toUtf8(), now);
        }
        else
        {
            model->setProperty(model->createdAtColumn().toUtf8(), now);
            model->setProperty(model->updatedAtColumn().toUtf8(), now);
        }
    }

    DatabaseManager* databaseManager;
};

#endif // REPOSITORY_H
