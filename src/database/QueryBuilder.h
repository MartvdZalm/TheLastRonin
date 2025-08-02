#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QStringList>
#include <QVariantMap>
#include <QSqlRecord>

class QueryBuilder
{
  public:
    explicit QueryBuilder(const QString& table);

    QueryBuilder& select(const QStringList& columns = {"*"});
    QueryBuilder& where(const QString& column, const QString& op, const QVariant& value);
    QueryBuilder& where(const QString& condition);
    QueryBuilder& orderBy(const QString& column, const QString& direction = "ASC");
    QueryBuilder& limit(int count);
    QSqlRecord first();
    QList<QSqlRecord> get();

    QSqlQuery execute();
    QString toSql() const;

    QMap<QString, QVariant> getBindings() const
    {
        return m_bindings;
    }

    static QueryBuilder table(const QString& tableName);

  private:
    QString m_table;
    QStringList m_selectColumns;
    QStringList m_whereConditions;
    QStringList m_joinClauses;
    QString m_orderByClause;
    QString m_groupByClause;
    QString m_havingClause;
    QString m_limitClause;
    QString m_offsetClause;
    QMap<QString, QVariant> m_bindings;
    int m_bindingCounter;

    QString generateBindingKey();
};

#endif // QUERYBUILDER_H
