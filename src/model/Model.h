#ifndef MODEL_H
#define MODEL_H

#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <QSqlRecord>

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

  public:
    explicit Model(QObject* parent = nullptr);
    virtual ~Model() = default;

    int id() const
    {
        return m_id;
    }

    void setId(int id);

    virtual void deserialize(const QSqlRecord& record) = 0;
    virtual QString getTableName() const = 0;
    virtual QStringList tableSchema() const = 0;
    virtual void fromVariantMap(const QVariantMap& map) = 0;
    virtual QVariantMap toVariantMap() const = 0;

    virtual bool isValid() const
    {
        return true;
    }

    virtual QStringList validationErrors() const
    {
        return {};
    }

    QStringList propertyNames() const;
    QVariant property(const QString& name) const;
    bool setProperty(const QString& name, const QVariant& value);

    virtual bool hasTimestamps() const
    {
        return false;
    }

    virtual QString createdAtColumn() const
    {
        return "created_at";
    }

    virtual QString updatedAtColumn() const
    {
        return "updated_at";
    }

  signals:
    void idChanged();
    void modelChanged();

  protected:
    void emitModelChanged()
    {
        emit modelChanged();
    }

  private:
    int m_id;
};

#endif // MODEL_H
