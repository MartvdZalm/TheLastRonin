#include "QueryBuilder.h"
#include <QSqlQuery>

QueryBuilder::QueryBuilder(const QString& table) : m_table(table), m_bindingCounter(0)
{
    m_selectColumns << "*";
}

QueryBuilder& QueryBuilder::select(const QStringList& columns)
{
    m_selectColumns = columns;
    return *this;
}

QueryBuilder& QueryBuilder::where(const QString& column, const QString& op, const QVariant& value)
{
    QString bindingKey = generateBindingKey();
    m_whereConditions << QString("%1 %2 :%3").arg(column, op, bindingKey);
    m_bindings[bindingKey] = value;
    return *this;
}

QueryBuilder& QueryBuilder::where(const QString& condition)
{
    m_whereConditions << condition;
    return *this;
}

QueryBuilder& QueryBuilder::orderBy(const QString& column, const QString& direction)
{
    m_orderByClause = QString("ORDER BY %1 %2").arg(column, direction);
    return *this;
}

QueryBuilder& QueryBuilder::limit(int count)
{
    m_limitClause = QString("LIMIT %1").arg(count);
    return *this;
}

QSqlQuery QueryBuilder::execute()
{
    QString sql = toSql();
    return DatabaseManager::instance().executeQuery(sql, m_bindings);
}

QString QueryBuilder::toSql() const
{
    QStringList parts;

    parts << QString("SELECT %1 FROM %2").arg(m_selectColumns.join(", "), m_table);

    if (!m_whereConditions.isEmpty())
    {
        parts << QString("WHERE %1").arg(m_whereConditions.join(" AND "));
    }

    if (!m_orderByClause.isEmpty())
    {
        parts << m_orderByClause;
    }

    if (!m_limitClause.isEmpty())
    {
        parts << m_limitClause;
    }

    return parts.join(" ");
}

QString QueryBuilder::generateBindingKey()
{
    return QString("bind_%1").arg(++m_bindingCounter);
}

QueryBuilder QueryBuilder::table(const QString& tableName)
{
    return QueryBuilder(tableName);
}

QList<QSqlRecord> QueryBuilder::get()
{
    QList<QSqlRecord> records;
    QSqlQuery query = execute();

    while (query.next())
    {
        records.append(query.record());
    }

    return records;
}

QSqlRecord QueryBuilder::first()
{
    limit(1);
    QSqlQuery query = execute();

    if (query.next())
    {
        return query.record();
    }

    return QSqlRecord();
}
