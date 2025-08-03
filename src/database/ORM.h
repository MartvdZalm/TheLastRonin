#ifndef ORM_H
#define ORM_H

#include "../model/Model.h"
#include "../repository/Repository.h"
#include <QMap>
#include <QObject>
#include <QStringList>
#include <functional>
#include <memory>
#include <typeinfo>

class ORM : public QObject
{
    Q_OBJECT

  public:
    static ORM& instance();

    template <typename T>
    void registerModel()
    {
        static_assert(std::is_base_of_v<Model, T>, "T must inherit from Model");

        auto model = std::make_unique<T>();
        QString tableName = model->getTableName();

        ModelInfo info;
        info.tableName = tableName;
        info.schema = model->tableSchema();
        info.factory = []() -> std::unique_ptr<Model> { return std::make_unique<T>(); };

        m_registeredModels[typeid(T).name()] = std::move(info);
        m_tableToType[tableName] = typeid(T).name();
    }

    bool initialize();
    bool createTables();
    bool tableExists(const QString& tableName);
    QStringList getRegisteredTables() const;
    std::unique_ptr<Model> createModel(const QString& tableName);

    template <typename T>
    std::shared_ptr<Repository<T>> getRepository()
    {
        QString typeName = typeid(T).name();
        if (!m_repositories.contains(typeName))
        {
            m_repositories[typeName] = std::make_shared<Repository<T>>();
        }
        return std::static_pointer_cast<Repository<T>>(m_repositories[typeName]);
    }

  signals:
    void tablesCreated();

  private:
    explicit ORM(QObject* parent = nullptr);
    ~ORM() = default;
    ORM(const ORM&) = delete;
    ORM& operator=(const ORM&) = delete;

    struct ModelInfo
    {
        QString tableName;
        QStringList schema;
        std::function<std::unique_ptr<Model>()> factory;
    };

    bool createTable(const QString& tableName, const QStringList& schema);

    QMap<QString, ModelInfo> m_registeredModels;
    QMap<QString, QString> m_tableToType;
    QMap<QString, std::shared_ptr<QObject>> m_repositories;
};

#define MODEL_REGISTRATION(ModelClass) \
    inline static const auto _ ## ModelClass ## _registrar = []() { \
        ORM::instance().registerModel<ModelClass>(); \
        return true; \
}()

#endif // ORM_H
