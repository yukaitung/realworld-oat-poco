#include "service/ArticleService.hpp"

#include <string>

oatpp::Object<ArticleExchangeDto> ArticleService::createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto) {
  std::string title = dto->article->title;
  std::string description = dto->article->description;
  std::string body = dto->article->body;
  // TODO : taglist
  OATPP_ASSERT_HTTP(!title.empty(), Status::CODE_422, "Missing title.");
  OATPP_ASSERT_HTTP(!description.empty(), Status::CODE_422, "Missing description.");
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "Missing body.");
}