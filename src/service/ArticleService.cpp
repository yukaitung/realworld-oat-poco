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

std::string ArticleService::replaceSpace(std::string &s) {
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

  // Create article
  auto article = articleModel.createArticle(id, slug, title, description, body, createTime);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Server error.");

  // Response data
  auto author = userModel.getProfileFromId(id);
  author->following = false;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::getArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");
  
  // TODO : taglist
  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(!authorId.empty(), Status::CODE_500, "Server error."); // Missing author id = error

  // Response data
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  auto articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");
  article->favourited = favouriteData.second;
  article->favouritesCount = favouriteData.first;
  auto author = userModel.getProfileFromId(authorId);
  if(!id.empty())
    author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  article->author = author;
  
  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::favouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_500, "Server error."); // Missing ArticleId = error

  // Favourite an article
  bool result = articleHasFavouriteModel.favouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  // Get favourite data
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  article->favourited = true;
  article->favouritesCount = favouriteData.first;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::unfavouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_500, "Server error."); // Missing ArticleId = error

  // Unavourite an article
  bool result = articleHasFavouriteModel.unfavouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  // Get favourite data
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  article->favourited = false;
  article->favouritesCount = favouriteData.first;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<TagJsonDto> ArticleService::getTags() {
  auto tags = tagModel.getTags();
  OATPP_ASSERT_HTTP(tags, Status::CODE_500, "Server error.");
  return tags;
}