
#ifndef USERDTO_HPP
#define USERDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

// User Object

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, token);
  DTO_FIELD(String, image);
  DTO_FIELD(String, bio);
};

class UserJsonDto : public oatpp::DTO {
  DTO_INIT(UserJsonDto, DTO)

  DTO_FIELD(oatpp::Object<UserDto>, user);
};

// User Registration

class UserRegDto : public oatpp::DTO {
  DTO_INIT(UserRegDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
};

class UserRegJsonDto : public oatpp::DTO {
  DTO_INIT(UserRegJsonDto, DTO)

  DTO_FIELD(oatpp::Object<UserRegDto>, user);
};

// User Authentication

class UserAuthDto : public oatpp::DTO {
  DTO_INIT(UserAuthDto, DTO)

  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
};

class UserAuthJsonDto : public oatpp::DTO {
  DTO_INIT(UserAuthJsonDto, DTO)

  DTO_FIELD(oatpp::Object<UserAuthDto>, user);
};

// User Update

class UserUpdateDto : public oatpp::DTO {
  DTO_INIT(UserUpdateDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
  DTO_FIELD(String, image);
  DTO_FIELD(String, bio);
};

class UserUpdateJsonDto : public oatpp::DTO {
  DTO_INIT(UserUpdateJsonDto, DTO)

  DTO_FIELD(oatpp::Object<UserUpdateDto>, user);
};

// User Profile

class UserProfileDto : public oatpp::DTO {
  DTO_INIT(UserProfileDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, image);
  DTO_FIELD(String, bio);
  DTO_FIELD(Boolean, following);
};

class UserProfileJsonDto : public oatpp::DTO {
  DTO_INIT(UserProfileJsonDto, DTO)

  DTO_FIELD(oatpp::Object<UserProfileDto>, profile);
};


#include OATPP_CODEGEN_END(DTO)

#endif // USERDTO_HPP