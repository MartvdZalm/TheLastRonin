#include "Model.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QMetaProperty>
#include "../database/ORM.h"

Model::Model(QObject* parent) : QObject(parent), m_id(-1) {}

void Model::setId(int id)
{
    if (m_id != id)
    {
        m_id = id;
        emit idChanged();
        emitModelChanged();
    }
}

QVariantMap Model::toVariantMap() const
{
    QVariantMap map;
    const QMetaObject* metaObj = metaObject();

    for (int i = 0; i < metaObj->propertyCount(); ++i)
    {
        QMetaProperty prop = metaObj->property(i);
        if (prop.isStored() && prop.isReadable())
        {
            QVariant value = prop.read(this);
            map[prop.name()] = value;
        }
    }

    return map;
}

void Model::fromVariantMap(const QVariantMap& map)
{
    const QMetaObject* metaObj = metaObject();

    for (int i = 0; i < metaObj->propertyCount(); ++i)
    {
        QMetaProperty prop = metaObj->property(i);
        if (prop.isStored() && prop.isWritable() && map.contains(prop.name()))
        {
            QVariant value = map[prop.name()];
            prop.write(this, value);
        }
    }
}

QStringList Model::propertyNames() const
{
    QStringList names;
    const QMetaObject* metaObj = metaObject();

    for (int i = 0; i < metaObj->propertyCount(); ++i)
    {
        QMetaProperty prop = metaObj->property(i);
        if (prop.isStored())
        {
            names << prop.name();
        }
    }

    return names;
}

QVariant Model::property(const QString& name) const
{
    return QObject::property(name.toLocal8Bit().constData());
}

bool Model::setProperty(const QString& name, const QVariant& value)
{
    bool result = QObject::setProperty(name.toLocal8Bit().constData(), value);
    if (result)
    {
        emitModelChanged();
    }
    return result;
}
