#include "service/UserService.hpp"

#include <string>

oatpp::Object<UserJsonDto> UserService::createUser(const oatpp::Object<UserRegJsonDto>& dto) {
  std::string email = dto->user->email;
  std::string username = dto->user->username;
  std::string password = dto->user->password;
  
  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "Missing email.");
  OATPP_ASSERT_HTTP(!username.empty(), Status::CODE_422, "Missing username.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "Missing password.");
  auto newUser = userModel.createUser(email, username, password);
  OATPP_ASSERT_HTTP(newUser != nullptr, Status::CODE_500, "Server error.");
  
  auto response = UserJsonDto::createShared();
  response->user = newUser;
  return response;
}

oatpp::Object<UserJsonDto> UserService::login(const oatpp::Object<UserAuthJsonDto>& dto) {
  std::string email = dto->user->email;
  std::string password = dto->user->password;

  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "Missing username.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "Missing password.");
  auto user = userModel.login(email, password);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Server error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}