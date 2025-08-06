#include "QueryBuilder.h"
#include "DatabaseManager.h"

QueryBuilder::QueryBuilder(const QString& table) : currentTable(table), bindingCounter(0), isDistinct(false)
{
    selectColumns << "*";
}

QueryBuilder& QueryBuilder::select(const QStringList& columns)
{
    selectColumns = columns;
    return *this;
}

QueryBuilder& QueryBuilder::select(const QString& column)
{
    selectColumns = QStringList{column};
    return *this;
}

QueryBuilder& QueryBuilder::addSelect(const QStringList& columns)
{
    selectColumns.append(columns);
    return *this;
}

QueryBuilder& QueryBuilder::distinct()
{
    isDistinct = true;
    return *this;
}

QueryBuilder& QueryBuilder::where(const QString& column, const QVariant& value)
{
    return where(column, "=", value);
}

QueryBuilder& QueryBuilder::where(const QString& column, const QString& op, const QVariant& value)
{
    QString bindingKey = generateBindingKey();
    addWhereCondition(QString("%1 %2 :%3").arg(column, op, bindingKey));
    bindings[bindingKey] = value;
    return *this;
}

QueryBuilder& QueryBuilder::where(const QString& condition)
{
    addWhereCondition(condition);
    return *this;
}

QueryBuilder& QueryBuilder::where(const std::function<void(QueryBuilder&)>& callback)
{
    QueryBuilder nestedQuery("");
    callback(nestedQuery);
    addWhereCondition(QString("(%1)").arg(nestedQuery.whereConditions.join(" AND ")));
    bindings.insert(nestedQuery.bindings);
    return *this;
}

QueryBuilder& QueryBuilder::orWhere(const QString& column, const QVariant& value)
{
    QString bindingKey = generateBindingKey();
    addWhereCondition(QString("%1 = :%2").arg(column, bindingKey), "OR");
    bindings[bindingKey] = value;
    return *this;
}

QueryBuilder& QueryBuilder::whereIn(const QString& column, const QList<QVariant>& values)
{
    QStringList bindingKeys;
    for (const auto& value : values)
    {
        QString key = generateBindingKey();
        bindingKeys << QString(":%1").arg(key);
        bindings[key] = value;
    }
    addWhereCondition(QString("%1 IN (%2)").arg(column, bindingKeys.join(",")));
    return *this;
}

QueryBuilder& QueryBuilder::whereNull(const QString& column)
{
    addWhereCondition(QString("%1 IS NULL").arg(column));
    return *this;
}

QueryBuilder& QueryBuilder::whereNotNull(const QString& column)
{
    addWhereCondition(QString("%1 IS NOT NULL").arg(column));
    return *this;
}

QueryBuilder& QueryBuilder::join(const QString& table, const QString& first, const QString& op, const QString& second)
{
    joinClauses << QString("JOIN %1 ON %2 %3 %4").arg(table, first, op, second);
    return *this;
}

QueryBuilder& QueryBuilder::leftJoin(const QString& table, const QString& first, const QString& op,
                                     const QString& second)
{
    joinClauses << QString("LEFT JOIN %1 ON %2 %3 %4").arg(table, first, op, second);
    return *this;
}

QueryBuilder& QueryBuilder::groupBy(const QStringList& columns)
{
    groupByClauses = columns;
    return *this;
}

QueryBuilder& QueryBuilder::orderBy(const QString& column, const QString& direction)
{
    orderByClause = QString("ORDER BY %1 %2").arg(column, direction);
    return *this;
}

QueryBuilder& QueryBuilder::latest(const QString& column)
{
    return orderBy(column, "DESC");
}

QueryBuilder& QueryBuilder::oldest(const QString& column)
{
    return orderBy(column, "ASC");
}

QueryBuilder& QueryBuilder::limit(int count)
{
    limitClause = QString("LIMIT %1").arg(count);
    return *this;
}

QueryBuilder& QueryBuilder::offset(int count)
{
    offsetClause = QString("OFFSET %1").arg(count);
    return *this;
}

QSqlQuery QueryBuilder::execute()
{
    QString sql = toSql();
    return DatabaseManager::instance().executeQuery(sql, bindings);
}

QString QueryBuilder::toSql() const
{
    QStringList parts;

    parts << "SELECT";
    if (isDistinct)
        parts << "DISTINCT";
    parts << selectColumns.join(", ") << "FROM" << currentTable;

    if (!joinClauses.isEmpty())
    {
        parts << joinClauses.join(" ");
    }

    if (!whereConditions.isEmpty())
    {
        parts << "WHERE" << whereConditions.join(" ");
    }

    if (!groupByClauses.isEmpty())
    {
        parts << "GROUP BY" << groupByClauses.join(", ");
    }

    if (!orderByClause.isEmpty())
    {
        parts << orderByClause;
    }

    if (!limitClause.isEmpty())
    {
        parts << limitClause;
    }

    if (!offsetClause.isEmpty())
    {
        parts << offsetClause;
    }

    return parts.join(" ");
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

QVariant QueryBuilder::value(const QString& column)
{
    QSqlRecord record = first();
    return record.value(column);
}

QList<QVariant> QueryBuilder::pluck(const QString& column)
{
    QList<QVariant> values;
    for (const auto& record : get())
    {
        values << record.value(column);
    }
    return values;
}

int QueryBuilder::count()
{
    select("COUNT(*) as count");
    return value("count").toInt();
}

QVariant QueryBuilder::max(const QString& column)
{
    select(QString("MAX(%1) as max_value").arg(column));
    return value("max_value");
}

QVariant QueryBuilder::min(const QString& column)
{
    select(QString("MIN(%1) as min_value").arg(column));
    return value("min_value");
}

QVariant QueryBuilder::sum(const QString& column)
{
    select(QString("SUM(%1) as sum_value").arg(column));
    return value("sum_value");
}

QVariant QueryBuilder::avg(const QString& column)
{
    select(QString("AVG(%1) as avg_value").arg(column));
    return value("avg_value");
}

QString QueryBuilder::generateBindingKey()
{
    return QString("bind_%1").arg(++bindingCounter);
}

void QueryBuilder::addWhereCondition(const QString& condition, const QString& connector)
{
    if (whereConditions.isEmpty())
    {
        whereConditions << condition;
    }
    else
    {
        whereConditions << QString("%1 %2").arg(connector, condition);
    }
}

QueryBuilder QueryBuilder::table(const QString& tableName)
{
    return QueryBuilder(tableName);
}

QSqlQuery QueryBuilder::insert(const QVariantMap& values)
{
    if (values.isEmpty())
    {
        qWarning() << "Attempted to insert with empty values";
        return QSqlQuery();
    }

    QStringList columns;
    QStringList placeholders;
    QMap<QString, QVariant> bindings;

    for (auto it = values.constBegin(); it != values.constEnd(); ++it)
    {
        QString key = it.key();
        QString placeholder = key;

        columns << key;
        placeholders << placeholder;
        bindings[placeholder] = it.value();
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (:%3)")
                      .arg(currentTable)
                      .arg(columns.join(", "))
                      .arg(placeholders.join(", :"));

    // qDebug() << "Executing SQL:" << sql << "with bindings:" << bindings;
    return DatabaseManager::instance().executeQuery(sql, bindings);
}

bool QueryBuilder::update(const QVariantMap& values)
{
    if (values.isEmpty() || whereConditions.isEmpty())
    {
        qWarning() << "Update failed: empty values or missing WHERE clause";
        return false;
    }

    QMap<QString, QVariant> allBindings = bindings;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
        allBindings[it.key()] = it.value();
    }

    QStringList setClauses;
    for (auto it = allBindings.begin(); it != allBindings.end(); ++it)
    {
        if (!it.key().startsWith("bind_"))
        {
            setClauses << QString("%1 = :%1").arg(it.key());
        }
    }

    if (setClauses.isEmpty())
    {
        qWarning() << "No fields to update";
        return false;
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE %3")
                      .arg(currentTable)
                      .arg(setClauses.join(", "))
                      .arg(whereConditions.join(" AND "));

    // qDebug() << "Executing update:" << sql << "with bindings:" << allBindings;
    QSqlQuery query = DatabaseManager::instance().executeQuery(sql, allBindings);

    return query.isActive() && query.numRowsAffected() > 0;
}
