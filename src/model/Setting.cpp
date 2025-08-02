#include "Setting.h"

Setting::Setting(QObject* parent) : Model(parent)
{
    createdAt = QDateTime::currentDateTime();
    updatedAt = createdAt;
}

void Setting::setKey(const QString& key)
{
    if (this->key != key)
    {
        this->key = key;
        emit keyChanged();
        emitModelChanged();
    }
}

void Setting::setValue(const QString& value)
{
    if (this->value != value)
    {
        this->value = value;
        emit valueChanged();
        emitModelChanged();
    }
}

void Setting::setCreatedAt(const QDateTime& createdAt)
{
    if (this->createdAt != createdAt)
    {
        this->createdAt = createdAt;
        emit createdAtChanged();
        emitModelChanged();
    }
}

void Setting::setUpdatedAt(const QDateTime& updatedAt)
{
    if (this->updatedAt != updatedAt)
    {
        this->updatedAt = updatedAt;
        emit updatedAtChanged();
        emitModelChanged();
    }
}

void Setting::deserialize(const QSqlRecord& record)
{
    setId(record.value("id").toInt());
    setKey(record.value("key").toString());
    setValue(record.value("value").toString());
    setCreatedAt(record.value("createdAt").toDateTime());
    setUpdatedAt(record.value("updatedAt").toDateTime());
}

QStringList Setting::tableSchema() const
{
    return {
        "id INTEGER PRIMARY KEY AUTOINCREMENT",
        "key TEXT NOT NULL UNIQUE",
        "value TEXT",
        "createdAt DATETIME DEFAULT CURRENT_TIMESTAMP",
        "updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP"
    };
}

bool Setting::isValid() const
{
    return !key.isEmpty() && !value.isEmpty();
}

QStringList Setting::validationErrors() const
{
    QStringList errors;
    if (key.isEmpty())
    {
        errors << "Key is required";
    }
    if (value.isEmpty())
    {
        errors << "Value is required";
    }
    return errors;
}