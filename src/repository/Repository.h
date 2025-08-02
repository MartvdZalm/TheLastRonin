#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "../database/DatabaseManager.h"
#include "../model/Model.h"
#include <QSqlRecord>
#include <QSqlTableModel>
#include <functional>
#include <memory>

template <typename T>
class Repository : public QObject
{
    static_assert(std::is_base_of_v<Model, T>, "T must inherit from Model");

  public:
    explicit Repository(QObject* parent = nullptr) : QObject(parent), databaseManager(&DatabaseManager::instance()) {}

    bool save(T* model);
    std::unique_ptr<T> find(int id);
    QList<std::unique_ptr<T>> findAll();

  private:
    bool insert(T* model);

    DatabaseManager* databaseManager;
};

#endif // REPOSITORY_H
