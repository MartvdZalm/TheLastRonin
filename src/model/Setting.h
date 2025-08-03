#ifndef SETTING_H
#define SETTING_H

#include "../model/Model.h"
#include <QDateTime>
#include <QString>
#include "../database/ORM.h"

class Setting : public Model
{
    Q_OBJECT
    Q_PROPERTY(QString key READ getKey WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QDateTime createdAt READ getCreatedAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ getUpdatedAt WRITE setUpdatedAt NOTIFY updatedAtChanged)

  public:
    explicit Setting(QObject* parent = nullptr);

    QString getKey() const
    {
        return key;
    }

    QString getValue() const
    {
        return value;
    }

    QDateTime getCreatedAt() const
    {
        return createdAt;
    }

    QDateTime getUpdatedAt() const
    {
        return updatedAt;
    }

    void setKey(const QString& key);
    void setValue(const QString& value);
    void setCreatedAt(const QDateTime& createdAt);
    void setUpdatedAt(const QDateTime& updatedAt);

    void deserialize(const QSqlRecord& record) override;
    void fromVariantMap(const QVariantMap& map) override;
    QVariantMap toVariantMap() const override;

    QString getTableName() const override
    {
        return "settings";
    }

    QStringList tableSchema() const override;

    bool hasTimestamps() const override
    {
        return true;
    }

    bool isValid() const override;
    QStringList validationErrors() const override;

  signals:
    void keyChanged();
    void valueChanged();
    void createdAtChanged();
    void updatedAtChanged();

  private:
    QString key;
    QString value;
    QDateTime createdAt;
    QDateTime updatedAt;
};
MODEL_REGISTRATION(Setting);

#endif // SETTING_H
