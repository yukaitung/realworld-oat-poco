#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "dto/UserDto.hpp"

#include <string>

class UserModel {
  private:
    std::string connectionName;
    std::string connectionString;

    std::string hashPassword(const std::string &passwordPlusSalt);

  public:
    UserModel();
    oatpp::Object<UserDto> createUser(std::string& email, std::string& username, std::string& password);
    oatpp::Object<UserDto> login(std::string &email, std::string &password);
    oatpp::Object<UserDto> getUser(std::string &username);
};

#endif // USERMODEL_HPP