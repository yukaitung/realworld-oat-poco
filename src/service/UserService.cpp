#include "service/UserService.hpp"

#include <string>
#include <iostream>

oatpp::Object<UserJsonDto> UserService::createUser(const oatpp::Object<UserRegJsonDto> &dto) {
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

oatpp::Object<UserJsonDto> UserService::login(const oatpp::Object<UserAuthJsonDto> &dto) {
  std::string email = dto->user->email;
  std::string password = dto->user->password;

  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "Missing email.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "Missing password.");
  auto user = userModel.login(email, password);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Server error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}

oatpp::Object<UserJsonDto> UserService::getUser(std::string &id) {
  OATPP_ASSERT_HTTP(!id.empty(), Status::CODE_422, "Missing id.");
  auto user = userModel.getUser(id);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Server error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}

oatpp::Object<UserJsonDto> UserService::updateUser(std::string &id, const oatpp::Object<UserUpdateJsonDto> &dto) {  
  std::string email = dto->user->email != nullptr ? dto->user->email : "";
  std::string username = dto->user->username != nullptr ? dto->user->username : "";
  std::string password = dto->user->password != nullptr ? dto->user->password : "";
  std::string bio = dto->user->bio != nullptr ? dto->user->bio : "";
  std::string image = dto->user->image != nullptr ? dto->user->image : "";

  auto user = userModel.updateUser(id, email, username, password, bio, image);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Server error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}