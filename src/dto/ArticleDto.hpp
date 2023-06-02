
#ifndef ARTICLEDTO_HPP
#define ARTICLEDTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

// Article Create Update Object

class ArticleExchangeDto : public oatpp::DTO {
  DTO_INIT(ArticleExchangeDto, DTO)

  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(String, tagList);
};

class ArticleExchangeJsonDto : public oatpp::DTO {
  DTO_INIT(ArticleExchangeJsonDto, DTO)

  DTO_FIELD(oatpp::Object<ArticleExchangeDto>, article);
};

#include OATPP_CODEGEN_END(DTO)

#endif // ARTICLEDTO_HPP