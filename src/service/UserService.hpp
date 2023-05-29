
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
  oatpp::Object<UserJsonDto> createUser(const oatpp::Object<UserRegJsonDto>& dto);
};

#endif //USERSERVICE_HPP
