#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "dto/UserDto.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"

#include <string>

class UserModel {
  private:
    std::string connectionName;
    std::string connectionString;

  public:
    UserModel();
    bool createUser(std::string& email, std::string& username, std::string& password);
};

#endif // USERMODEL_HPP