// #ifndef USER_H
// #define USER_H
//
// #include "Model.h"
// #include <QString>
//
// class User : public Model
//{
//     Q_OBJECT
//     Q_PROPERTY(QString email READ setEmail WRITE setEmail NOTIFY emailChanged)
//     Q_PROPERTY(QString password READ setPassword WRITE setPassword NOTIFY passwordChanged)
//     Q_PROPERTY(QString clientId READ setClientId WRITE setClientId NOTIFY clientIdChanged)
//
//   public:
//     explicit User(QObject* parent = nullptr);
//
//     QString setEmail() const
//     {
//         return email;
//     }
//
//     QString setPassword() const
//     {
//         return password;
//     }
//
//     QString setClientId() const
//     {
//         return clientId;
//     }
//
//     void setEmail(const QString& email);
//     void setPassword(const QString& password);
//     void setClientId(const QString& clientId);
//
//     QString getTableName() const override
//     {
//         return "users";
//     }
//
//     QStringList tableSchema() const override;
//
//     bool isValid() const override;
//     QStringList validationErrors() const override;
//
//     bool isValidEmail() const;
//     void hashPassword();
//
//   signals:
//     void emailChanged();
//     void passwordChanged();
//     void clientIdChanged();
//
//   private:
//     QString email;
//     QString password;
//     QString clientId;
// };
//
// #endif // USER_H
