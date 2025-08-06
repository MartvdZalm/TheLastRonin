// #include "UserRepository.h"
//
// #include <QCryptographicHash>
// #include <QDebug>
// #include <QSqlError>
// #include <QSqlQuery>
//
// UserRepository::UserRepository() : db(DatabaseManager::instance()) {}
//
// bool UserRepository::insertUser(const User& user)
//{
//     QSqlQuery query;
//     query.prepare(R"(
//         INSERT INTO users (email, password, client_id)
//         VALUES (:email, :password, :client_id)
//     )");
//
//     QByteArray hash = QCryptographicHash::hash(user.password.toUtf8(), QCryptographicHash::Sha256);
//     QString hashedPassword = hash.toHex();
//
//     query.bindValue(":email", user.email);
//     query.bindValue(":password", hashedPassword);
//     query.bindValue(":client_id", user.clientId);
//
//     if (!query.exec())
//     {
//         qWarning() << "Failed to insert user:" << query.lastError().text();
//         return false;
//     }
//
//     return true;
// }
