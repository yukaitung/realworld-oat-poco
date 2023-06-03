#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "dto/UserDto.hpp"

#include <string>

class UserModel {
  private:
    std::string hashPassword(const std::string &passwordPlusSalt);

  public:
    UserModel() {};
    oatpp::Object<UserDto> createUser(std::string &email, std::string &username, std::string &password);
    oatpp::Object<UserDto> login(std::string &email, std::string &password);
    oatpp::Object<UserDto> getUser(std::string &id);
    oatpp::Object<UserDto> updateUser(std::string &id, std::string &email, std::string &username, std::string &password, std::string &bio, std::string &image);
};

#endif // USERMODEL_HPP