#include "model/ArticleModel.hpp"
#include "helper/Database.hpp"

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

std::string ArticleModel::timeTz(std::string &time) {
  std::string tz = time + ".000Z";
  tz[10] = 'T';
  return tz; 
}

oatpp::Object<ArticleDto> ArticleModel::createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::string &tagsStr, std::string &createTime) {
  try {
    // Insert article
    Session session(Database::getPool()->get());
    session << "INSERT INTO articles (user_id, slug, title, description, body, tag_list, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", use(userId), use(slug), use(title), use(description), use(body), use(tagsStr), use(createTime), use(createTime), now;

    auto article = ArticleDto::createShared();
    article->slug = slug;
    article->title = title;
    article->description = description;
    article->body = body;
    std::string tz = timeTz(createTime);
    article->createdAt = tz;
    article->updatedAt = tz;
    article->favourited = false;
    article->favouritesCount = 0;
    return article;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return nullptr;
  }
}

std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> ArticleModel::getArticle(std::string &slug) {
  Poco::Nullable<std::string> retrunArticleId;
  Poco::Nullable<std::string> retrunUserId;
  Poco::Nullable<std::string> retrunSlug;
  Poco::Nullable<std::string> retrunTitle;
  Poco::Nullable<std::string> retrunDescription;
  Poco::Nullable<std::string> retrunBody;
  Poco::Nullable<std::string> retrunTagList;
  Poco::Nullable<std::string> retrunCreatedAt;
  Poco::Nullable<std::string> retrunUpdateddAt;
  
  try {
    // Insert article
    Session session(Database::getPool()->get());
    session << "SELECT CAST(id AS char), CAST(user_id AS char), slug, title, description, body, CAST(tag_list AS char), CAST(created_at AS char), CAST(updated_at AS char) FROM articles WHERE slug = ?", into(retrunArticleId), into(retrunUserId), into(retrunSlug), into(retrunTitle), into(retrunDescription), into(retrunBody), into(retrunTagList), into(retrunCreatedAt), into(retrunUpdateddAt), use(slug), now;
    
    auto article = ArticleDto::createShared();
    if(!retrunSlug.isNull()) {
      article->slug = retrunSlug.value();
      if(!retrunTitle.isNull())
        article->title = retrunTitle.value();
      if(!retrunDescription.isNull())
        article->description = retrunDescription.value();
      if(!retrunBody.isNull())
        article->body = retrunBody.value();
      if(retrunTagList.isNull()) {
        retrunTagList = "[]";
      }
      article->favourited = false;
      article->favouritesCount = 0;
      article->createdAt = timeTz(retrunCreatedAt);
      article->updatedAt = timeTz(retrunUpdateddAt);
    }
    
    return {article , !retrunArticleId.isNull() ? retrunArticleId.value() : "", !retrunUserId.isNull() ? retrunUserId.value() : "", !retrunTagList.isNull() ? retrunTagList.value() : ""};
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {nullptr , "", "", ""};
  }
}

std::tuple<oatpp::Vector<oatpp::Object<ArticleDto>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> ArticleModel::getArticles(unsigned int limit, unsigned int offset, std::string &tagId, std::string &authorId, std::string &favouritedBy, bool feed, std::string &userId) {
  int conditionCount = 0;
  if(!tagId.empty())
    conditionCount++;
  if(!authorId.empty()) 
    conditionCount++;
  if(!favouritedBy.empty())
    conditionCount++;
  if(feed && !userId.empty())
    conditionCount++;
  
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT CAST(id AS char), CAST(articles.user_id AS char), slug, title, description, body, CAST(tag_list AS char), CAST(created_at AS char), CAST(updated_at AS char) FROM articles";
    if(!favouritedBy.empty()) // favouritedBy requires join
      select << " INNER JOIN articles_has_favourites ON articles.id = articles_has_favourites.article_id";
    if(feed && !userId.empty()) // feed requires join
      select << " INNER JOIN users_has_followers ON articles.user_id = users_has_followers.user_id";
    if(conditionCount > 0)
      select << " WHERE";
    if(!tagId.empty()) {
      select << " JSON_CONTAINS(tag_list, ?)", use(tagId);
    }
    if(!authorId.empty()) {
      if(conditionCount > 1)
        select << " AND";
      select << " user_id = ?", use(authorId);
    } 
    if(!favouritedBy.empty()) {
      if(conditionCount > 1)
        select << " AND";
      select << " articles_has_favourites.user_id = ?", use(favouritedBy);
    }
    if(feed && !userId.empty()) {
      if(conditionCount > 1)
        select << " AND";
      select << " users_has_followers.follower_id = ?", use(userId);
    }
    select << " ORDER BY created_at DESC LIMIT ?, ?", use(offset), use(limit);
    select.execute();
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    oatpp::Vector<oatpp::Object<ArticleDto>> articles = oatpp::Vector<oatpp::Object<ArticleDto>>::createShared();
    articles->resize(rowCount);
    std::vector<std::string> articleIds(rowCount);
    std::vector<std::string> authorUsers(rowCount);
    std::vector<std::string> tagsJson(rowCount);
    for(int i = 0; i < rowCount; i++) {
      // Article
      auto article = ArticleDto::createShared();
      article->slug = rs.value(2, i).toString();
      if(!rs.value(3, i).isEmpty())
        article->title = rs.value(3, i).toString();
      if(!rs.value(4, i).isEmpty())
        article->description = rs.value(4, i).toString();
      if(!rs.value(5, i).isEmpty())
        article->body = rs.value(5, i).toString();
      article->favourited = false;
      article->favouritesCount = 0;
      std::string createdAtStr = rs.value(7, i).toString();
      std::string updatedAtStr = rs.value(8, i).toString();
      article->createdAt = timeTz(createdAtStr);
      article->updatedAt = timeTz(updatedAtStr);
      articles->at(i) = article;

      articleIds[i] = rs.value(0, i).toString();
      authorUsers[i] = rs.value(1, i).toString();
      if(rs.value(6, i).isEmpty()) {
        tagsJson[i] = "[]";
      }
      else {
        tagsJson[i] = rs.value(6, i).toString();
      }
    }
    return {articles, articleIds, authorUsers, tagsJson};
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {nullptr , {}, {}, {}};
  }
}

bool ArticleModel::updateArticle(std::string &slug, std::string &newSlug, std::string &title, std::string &description, std::string &body, std::string &tagsStr, std::string &updateTime) {
  try {
    Session session(Database::getPool()->get());
    Statement updateStatment(session);
    updateStatment << "UPDATE articles SET";
    if(!newSlug.empty())
      updateStatment << " slug = ?,", use(newSlug);
    if(!title.empty())
      updateStatment << " title = ?,",use(title);
    if(!description.empty())
      updateStatment << " description = ?,", use(description);
    if(!tagsStr.empty())
      updateStatment << " tag_list = ?,", use(tagsStr);
    if(!body.empty())
      updateStatment << " body = ?,", use(body);
    updateStatment << " updated_at = ? WHERE slug = ?", use(updateTime), use(slug);
    updateStatment.execute();
    
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool ArticleModel::deleteArticle(std::string &slug) {
  try {
    Session session(Database::getPool()->get());
    session << "DELETE FROM articles WHERE slug = ?", use(slug), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

std::string ArticleModel::getArticleIdFromSlug(std::string &slug) {
  Poco::Nullable<std::string> retrunArticleId;
  
  try {
    Session session(Database::getPool()->get());
    session << "SELECT CAST(id AS char) FROM articles WHERE slug = ?", into(retrunArticleId), use(slug), now;

    return !retrunArticleId.isNull() ? retrunArticleId.value() : "";
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return "";
  }
}