#include "model/ArticleHasFavouriteModel.hpp"
#include "helper/DatabaseHelper.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

bool ArticleHasFavouriteModel::favouriteArticle(std::string &articleId, std::string &userId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "INSERT INTO articles_has_favourites (article_id, user_id) VALUES (?, ?)", use(articleId), use(userId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool ArticleHasFavouriteModel::unfavouriteArticle(std::string &articleId, std::string &userId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "DELETE FROM articles_has_favourites WHERE (article_id = ?) and (user_id = ?)", use(articleId), use(userId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool ArticleHasFavouriteModel::deleteFavouriteForArticle(std::string &articleId) {
try {
    Session session(DatabaseHelper::getSession());
    session << "DELETE FROM articles_has_favourites WHERE (article_id = ?)", use(articleId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

std::unordered_map<std::string, std::pair<unsigned int, bool>> ArticleHasFavouriteModel::getArticlefavouriteData(std::vector<std::string> &articleId, std::string &userId) {
  if(articleId.empty())
    return {};

  std::unordered_map<std::string, std::pair<unsigned int, bool>> favourtieData;

  try {
    Session session(DatabaseHelper::getSession());
    Statement select(session);
    select << "SELECT article_id, COUNT(*), COUNT(CASE WHEN user_id = ? THEN 1 END) FROM articles_has_favourites WHERE article_id IN(", use(userId);
    for(int i = 0; i < articleId.size(); i++) {
      select << "?", use(articleId[i]);
      if(i < articleId.size() - 1)
        select << ",";
    }
    select << ") GROUP BY article_id";
    select.execute();
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      std::string id = rs.value(0, i).toString();
      unsigned int favouriteCount = rs.value(1, i);
      bool userFavourite = rs.value(2, i) >= 1;
      favourtieData.insert({id, {favouriteCount, userFavourite}});
    }

    if(favourtieData.empty()) { // No favourite record in database, append a dummy record
      favourtieData.insert({articleId[0], {0, false}});
    }

    return favourtieData;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {};
  } 
}

std::vector<std::string> ArticleHasFavouriteModel::getUserFavourite(std::string &userId) {
  std::vector<std::string> favouriteArticles;

  try {
    Session session(DatabaseHelper::getSession());
    Statement select(session);
    select << "SELECT article_id FROM articles_has_favourites WHERE user_id = ?", use(userId);

    select.execute();
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();
    favouriteArticles.resize(rowCount);

    for(int i = 0; i < rowCount; i++) {
      favouriteArticles[i] = rs.value(0, i).toString();
    }

    return favouriteArticles;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleHasFavouriteModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {};
  }
}