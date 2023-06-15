#ifndef STATUSDTO_HPP
#define STATUSDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ErrorBodyDto : public oatpp::DTO {
  DTO_INIT(ErrorBodyDto, DTO)

  DTO_FIELD(oatpp::Vector<String>, body);
};

class ErrorJsonDto : public oatpp::DTO {
  DTO_INIT(ErrorJsonDto, DTO)

  DTO_FIELD(oatpp::Object<ErrorBodyDto>, errors);
};

#include OATPP_CODEGEN_END(DTO)

#endif // ARTICLEDTO_HPP