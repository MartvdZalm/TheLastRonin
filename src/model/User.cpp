//#include "User.h"
//#include <QCryptographicHash>
//#include <QRegularExpression>
//
//User::User(QObject* parent) : Model(parent) {}
//
//void User::setEmail(const QString& email)
//{
//    if (this->email != email)
//    {
//        this->email = email;
//        emit emailChanged();
//        emitModelChanged();
//    }
//}
//
//void User::setPassword(const QString& password)
//{
//    if (this->password != password)
//    {
//        this->password = password;
//        emit passwordChanged();
//        emitModelChanged();
//    }
//}
//
//void User::setClientId(const QString& clientId)
//{
//    if (this->clientId != clientId)
//    {
//        this->clientId = clientId;
//        emit clientIdChanged();
//        emitModelChanged();
//    }
//}
//
//QStringList User::tableSchema() const
//{
//    return {"id INTEGER PRIMARY KEY AUTOINCREMENT", "email TEXT UNIQUE NOT NULL", "password TEXT NOT NULL",
//            "clientId TEXT"};
//}
//
//bool User::isValid() const
//{
//    return isValidEmail() && !password.isEmpty();
//}
//
//QStringList User::validationErrors() const
//{
//    QStringList errors;
//    if (!isValidEmail())
//    {
//        errors << "Invalid email format";
//    }
//    if (password.isEmpty())
//    {
//        errors << "Password is required";
//    }
//    if (password.length() < 6)
//    {
//        errors << "Password must be at least 6 characters";
//    }
//    return errors;
//}
//
//bool User::isValidEmail() const
//{
//    QRegularExpression emailRegex("^[\\w\\.-]+@[\\w\\.-]+\\.[a-zA-Z]{2,}$");
//    return emailRegex.match(email).hasMatch();
//}
//
//void User::hashPassword()
//{
//    if (!password.isEmpty())
//    {
//        QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
//        password = hash.toHex();
//    }
//}
