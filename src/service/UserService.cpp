#include "service/UserService.hpp"

oatpp::Object<UserJsonDto> UserService::createUser(const oatpp::Object<UserRegJsonDto> &dto) {
  std::string email = dto->user->email;
  std::string username = dto->user->username;
  std::string password = dto->user->password;
  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "The email is missing.");
  OATPP_ASSERT_HTTP(!username.empty(), Status::CODE_422, "The username is missing.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "The password is missing.");

  // Validate the email and username
  auto validate = userModel.validateUser(email, username);
  OATPP_ASSERT_HTTP(validate.first == false, Status::CODE_422, "The email is taken by other user.");
  OATPP_ASSERT_HTTP(validate.second == false, Status::CODE_422, "The username is taken by other user.");
  // Create a user
  auto newUser = userModel.createUser(email, username, password);
  OATPP_ASSERT_HTTP(newUser != nullptr, Status::CODE_500, "Internal Server Error.");
  
  auto response = UserJsonDto::createShared();
  response->user = newUser;
  return response;
}

oatpp::Object<UserJsonDto> UserService::login(const oatpp::Object<UserAuthJsonDto> &dto) {
  std::string email = dto->user->email;
  std::string password = dto->user->password;
  OATPP_ASSERT_HTTP(!email.empty(), Status::CODE_422, "The email is missing.");
  OATPP_ASSERT_HTTP(!password.empty(), Status::CODE_422, "The password is missing.");

  // Validate the email
  auto validate = userModel.validateUser(email, email); // Just a dummy
  OATPP_ASSERT_HTTP(validate.first == true, Status::CODE_404, "The email could not be found.");
  // Login
  auto user = userModel.login(email, password);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Internal Server Error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}

oatpp::Object<UserJsonDto> UserService::getUser(std::string &id) {
  OATPP_ASSERT_HTTP(!id.empty(), Status::CODE_422, "The user id is missing, that could be some error in authorization.");
  auto user = userModel.getUser(id);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Internal Server Error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}

oatpp::Object<UserJsonDto> UserService::updateUser(std::string &id, const oatpp::Object<UserUpdateJsonDto> &dto) {
  OATPP_ASSERT_HTTP(!id.empty(), Status::CODE_422, "The user id is missing, that could be some error in authorization.");
  std::string email = dto->user->email != nullptr ? dto->user->email : "";
  std::string username = dto->user->username != nullptr ? dto->user->username : "";
  std::string password = dto->user->password != nullptr ? dto->user->password : "";
  std::string bio = dto->user->bio != nullptr ? dto->user->bio : "";
  std::string image = dto->user->image != nullptr ? dto->user->image : "";

  // Validate the email / username if the user wants to upadte it
  if(!email.empty() || !username.empty()) {
    auto validate = userModel.validateUser(email, username);
    if(!email.empty())
      OATPP_ASSERT_HTTP(validate.first == false, Status::CODE_422, "The email is taken by other user.");
    if(!username.empty())
      OATPP_ASSERT_HTTP(validate.second == false, Status::CODE_422, "The username is taken by other user.");
  }
  auto user = userModel.updateUser(id, email, username, password, bio, image);
  OATPP_ASSERT_HTTP(user != nullptr, Status::CODE_500, "Internal Server Error.");

  auto response = UserJsonDto::createShared();
  response->user = user;
  return response;
}