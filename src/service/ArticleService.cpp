#include "service/ArticleService.hpp"

#include "Poco/Timestamp.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"

#include <regex>

std::string &ArticleService::removeHeadSpace(std::string &s, const char *t) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

std::string &ArticleService::removeTailSpace(std::string &s, const char *t) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

std::string &ArticleService::removeBothSpace(std::string &s, const char *t){
  return removeHeadSpace(removeTailSpace(s, t), t);
}

std::string &ArticleService::replaceSpace(std::string &s) {
  std::regex r("\\s");
  s = std::regex_replace(s, r, "-");
  return s;
}

oatpp::Object<ArticleJsonDto> ArticleService::createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto) {
  std::string title = dto->article->title;
  title = removeBothSpace(title);
  std::string description = dto->article->description;
  std::string body = dto->article->body;
  // TODO : taglist
  OATPP_ASSERT_HTTP(!title.empty(), Status::CODE_422, "Missing title.");
  OATPP_ASSERT_HTTP(!description.empty(), Status::CODE_422, "Missing description.");
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "Missing body.");
  
  // slug = title replacing space + user id + timestamp
  Poco::LocalDateTime dateTime;
  std::string slug = title;
  slug = replaceSpace(slug) + "-" + id + std::to_string(dateTime.timestamp().epochMicroseconds());
  std::string createTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());

  auto article = articleModel.createArticle(id, slug, title, description, body, createTime);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Server error.");

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}