#ifndef ARTICLESERVICE_HPP
#define ARTICLESERVICE_HPP

#include "model/ArticleHasFavouriteModel.hpp"
#include "model/ArticleModel.hpp"
#include "model/TagModel.hpp"
#include "model/UserHasFollowerModel.hpp"
#include "model/UserModel.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

class ArticleService {
private:
  ArticleHasFavouriteModel articleHasFavouriteModel;
  ArticleModel articleModel;
  TagModel tagModel;
  UserHasFollowerModel userHasFollowerModel;
  UserModel userModel;
  typedef oatpp::web::protocol::http::Status Status;
  std::string &removeHeadSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string &removeTailSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string &removeBothSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string replaceSpace(std::string &s);

public:
  oatpp::Object<ArticleJsonDto> createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto);
  oatpp::Object<ArticleJsonDto> getArticle(std::string &id, std::string &slug);

  oatpp::Object<ArticleJsonDto> favouriteArticle(std::string &id, std::string &slug);
  oatpp::Object<ArticleJsonDto> unfavouriteArticle(std::string &id, std::string &slug);

  oatpp::Object<TagJsonDto> getTags();
};

#endif // ARTICLESERVICE_HPP