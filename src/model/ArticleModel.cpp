#include "model/ArticleModel.hpp"
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

std::string ArticleModel::timeTz(std::string &time) {
  std::string tz = time + ".000Z";
  tz[10] = 'T';
  return tz; 
}

oatpp::Object<ArticleDto> ArticleModel::createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::string &createTime) {
  try {
    // Insert article
    Session session(Database::getPool()->get());
    session << "INSERT INTO articles (user_id, slug, title, description, body, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?)", use(userId), use(slug), use(title), use(description), use(body), use(createTime), use(createTime), now;

    auto article = ArticleDto::createShared();
    article->slug = slug;
    article->title = title;
    article->description = description;
    article->body = body;
    std::string tz = timeTz(createTime);
    article->createdAt = tz;
    article->updatedAt = tz;
    article->favorited = false;
    article->favoritesCount = 0;
    return article;
  }
  catch(Exception& exp) {
    OATPP_LOGE("ArticleModel", exp.displayText().c_str());
    return nullptr;
  }
  
}