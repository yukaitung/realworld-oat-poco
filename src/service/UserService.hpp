
#ifndef USERSERVICE_HPP
#define USERSERVICE_HPP

#include "dto/UserDto.hpp"
#include "model/UserModel.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

class UserService {
private:
  typedef oatpp::web::protocol::http::Status Status;
  UserModel userModel;

public:
  oatpp::Object<UserJsonDto> createUser(const oatpp::Object<UserRegJsonDto> &dto);
  oatpp::Object<UserJsonDto> login(const oatpp::Object<UserAuthJsonDto> &dto);
  oatpp::Object<UserJsonDto> getUser(std::string &id);
  oatpp::Object<UserJsonDto> updateUser(std::string &id, const oatpp::Object<UserUpdateJsonDto> &dto);
};

#endif //USERSERVICE_HPP
