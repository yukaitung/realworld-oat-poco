
#ifndef USERDTO_HPP
#define USERDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
  DTO_FIELD(String, token);
  DTO_FIELD(String, image);
  DTO_FIELD(String, bio);
};

#include OATPP_CODEGEN_END(DTO)

#endif // USERDTO_HPP