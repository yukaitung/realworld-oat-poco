#ifndef ARTICLESERVICE_HPP
#define ARTICLESERVICE_HPP

#include "model/ArticleHasFavouriteModel.hpp"
#include "model/ArticleModel.hpp"
#include "model/CommentModel.hpp"
#include "model/TagModel.hpp"
#include "model/UserHasFollowerModel.hpp"
#include "model/UserModel.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

#include <vector>
#include <string>
#include <regex>

class ArticleService {
private:
  ArticleHasFavouriteModel articleHasFavouriteModel;
  ArticleModel articleModel;
  CommentModel commentModel;
  TagModel tagModel;
  UserHasFollowerModel userHasFollowerModel;
  UserModel userModel;
  const std::regex splitJsonArrRegex{R"__(\[\]|\[\s*|\s*\]|\s*,\s*)__"};

  typedef oatpp::web::protocol::http::Status Status;

  std::string &removeHeadSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string &removeTailSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string &removeBothSpace(std::string &s, const char *t = " \t\n\r\f\v");
  std::string replaceSpace(std::string &s);
  std::vector<std::string> splitStr(const std::string &s, const std::regex &sep_regex);

public:
  oatpp::Object<ArticleJsonDto> createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto);
  oatpp::Object<ArticleJsonDto> getArticle(std::string &id, std::string &slug);
  oatpp::Object<ArticlesJsonDto> getArticles(std::string &id, unsigned int limit, unsigned int offset, std::string &tag, std::string &author, std::string &favouritedBy, bool feed = false);
  oatpp::Object<ArticleJsonDto> updateArticle(std::string &id, std::string &slug, const oatpp::Object<ArticleExchangeJsonDto> &dto);
  oatpp::Object<ArticleJsonDto> deleteArticle(std::string &id, std::string &slug);

  oatpp::Object<ArticleJsonDto> favouriteArticle(std::string &id, std::string &slug);
  oatpp::Object<ArticleJsonDto> unfavouriteArticle(std::string &id, std::string &slug);

  oatpp::Object<CommentJsonDto> createComment(std::string &id, std::string &slug, const oatpp::Object<CommentJsonDto> &dto);
  oatpp::Object<CommentsJsonDto> getComments(std::string &id, std::string &slug);

  oatpp::Object<TagJsonDto> getTags();
};

#endif // ARTICLESERVICE_HPP