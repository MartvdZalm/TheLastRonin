#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariantMap>
#include <functional>

class QueryBuilder
{
  public:
    explicit QueryBuilder(const QString& table);

    QueryBuilder& select(const QStringList& columns = {"*"});
    QueryBuilder& select(const QString& column);
    QueryBuilder& addSelect(const QStringList& columns);
    QueryBuilder& distinct();
    QueryBuilder& where(const QString& column, const QVariant& value);
    QueryBuilder& where(const QString& column, const QString& op, const QVariant& value);
    QueryBuilder& where(const QString& condition);
    QueryBuilder& where(const std::function<void(QueryBuilder&)>& callback);
    QueryBuilder& orWhere(const QString& column, const QVariant& value);
    QueryBuilder& whereIn(const QString& column, const QList<QVariant>& values);
    QueryBuilder& whereNull(const QString& column);
    QueryBuilder& whereNotNull(const QString& column);
    QueryBuilder& join(const QString& table, const QString& first, const QString& op, const QString& second);
    QueryBuilder& leftJoin(const QString& table, const QString& first, const QString& op, const QString& second);
    QueryBuilder& groupBy(const QStringList& columns);
    QueryBuilder& orderBy(const QString& column, const QString& direction = "ASC");
    QueryBuilder& latest(const QString& column = "created_at");
    QueryBuilder& oldest(const QString& column = "created_at");
    QueryBuilder& limit(int count);
    QueryBuilder& offset(int count);

    QueryBuilder& take(int count)
    {
        return limit(count);
    }

    QueryBuilder& skip(int count)
    {
        return offset(count);
    }

    QSqlQuery insert(const QVariantMap& values);
    bool update(const QVariantMap& values);
    int count();
    QVariant max(const QString& column);
    QVariant min(const QString& column);
    QVariant sum(const QString& column);
    QVariant avg(const QString& column);
    QSqlRecord first();
    QList<QSqlRecord> get();
    QVariant value(const QString& column);
    QList<QVariant> pluck(const QString& column);
    QSqlQuery execute();
    QString toSql() const;

    QMap<QString, QVariant> getBindings() const
    {
        return bindings;
    }

    static QueryBuilder table(const QString& tableName);

  private:
    QString currentTable;
    bool isDistinct = false;
    QStringList selectColumns;
    QStringList joinClauses;
    QStringList whereConditions;
    QStringList groupByClauses;
    QString orderByClause;
    QString limitClause;
    QString offsetClause;
    QMap<QString, QVariant> bindings;
    int bindingCounter;

    QString generateBindingKey();
    void addWhereCondition(const QString& condition, const QString& connector = "AND");
};

#endif // QUERYBUILDER_H
