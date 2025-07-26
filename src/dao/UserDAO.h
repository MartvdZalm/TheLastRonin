#ifndef USERDAO_H
#define USERDAO_H

#include "../database/DatabaseManager.h"
#include "../model/User.h"

class UserDAO
{
  public:
    UserDAO();

    bool insertUser(const User& user);

  private:
    DatabaseManager& db;
};

#endif // USERDAO_H
