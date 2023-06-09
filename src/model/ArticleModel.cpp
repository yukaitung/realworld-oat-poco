#include "model/ArticleModel.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
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
    OATPP_LOGE("ArticleModel", exp.displayText().c_str());
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
    article->createdAt = timeTz(retrunCreatedAt);
    article->updatedAt = timeTz(retrunUpdateddAt);
    
    return {article , retrunArticleId.value(), retrunUserId.value(), retrunTagList.value()};
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", exp.displayText().c_str());
    return {nullptr , "", "", ""};
  }
}