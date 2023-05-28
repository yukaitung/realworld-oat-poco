#include "service/UserService.hpp"

#include <string>

using namespace std;

oatpp::Object<UserDto> UserService::createUser(const oatpp::Object<UserDto>& userDto) {
  string email = userDto->email;
  string username = userDto->username;
  string password = userDto->password;
  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "Missing email.");
  OATPP_ASSERT_HTTP(!username.empty(), Status::CODE_422, "Missing username.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "Missing password.");
  bool result = userModel.createUser(email, username, password);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");
  return userDto;
}