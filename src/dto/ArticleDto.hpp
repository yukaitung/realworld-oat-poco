
#ifndef ARTICLEDTO_HPP
#define ARTICLEDTO_HPP

#include "dto/UserDto.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

// Article Object

class ArticleDto : public oatpp::DTO {
  DTO_INIT(ArticleDto, DTO)

  DTO_FIELD(String, slug);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, body);
  DTO_FIELD(String, tagList);
  DTO_FIELD(String, createdAt);
  DTO_FIELD(String, updatedAt);
  DTO_FIELD(Boolean, favourited, "favorited");
  DTO_FIELD(Int32, favouritesCount, "favoritesCount");
  DTO_FIELD(oatpp::Object<UserProfileDto>, author);
};

class ArticleJsonDto : public oatpp::DTO {
  DTO_INIT(ArticleJsonDto, DTO)

  DTO_FIELD(oatpp::Object<ArticleDto>, article);
};

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

// Tags

class TagJsonDto : public oatpp::DTO {
  DTO_INIT(TagJsonDto, DTO)

  DTO_FIELD(oatpp::Vector<String>, tags);
};

#include OATPP_CODEGEN_END(DTO)

#endif // ARTICLEDTO_HPP