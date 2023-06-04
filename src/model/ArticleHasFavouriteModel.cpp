#include "model/ArticleHasFavouriteModel.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

bool ArticleHasFavouriteModel::favouriteArticle(std::string &articleId, std::string &userId) {
  try {
    Session session(Database::getPool()->get());
    session << "INSERT INTO articles_has_favourites (article_id, user_id) VALUES (?, ?)", use(articleId), use(userId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", exp.displayText().c_str());
    return false;
  }
}

bool ArticleHasFavouriteModel::unfavouriteArticle(std::string &articleId, std::string &userId) {
  try {
    Session session(Database::getPool()->get());
    session << "DELETE FROM articles_has_favourites WHERE (article_id = ?) and (user_id = ?)", use(articleId), use(userId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", exp.displayText().c_str());
    return false;
  }
}

std::pair<unsigned int, bool> ArticleHasFavouriteModel::getArticlefavouriteData(std::string &articleId, std::string &userId) {
  Poco::Nullable<unsigned int> retrunFavouriteCount;
  Poco::Nullable<unsigned int> retrunUserFavourite;

  try {
    Session session(Database::getPool()->get());
    session << "SELECT COUNT(*), COUNT(CASE WHEN user_id = ? THEN 1 END) FROM articles_has_favourites WHERE article_id = ?;", into(retrunFavouriteCount), into(retrunUserFavourite), use(userId), use(articleId), now;
    return {retrunFavouriteCount.value(), retrunUserFavourite.value() >= 1};
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", exp.displayText().c_str());
    return {-1, false};
  } 
}