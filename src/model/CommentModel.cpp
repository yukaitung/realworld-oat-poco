#include "model/CommentModel.hpp"
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

std::string CommentModel::timeTz(std::string &time) {
  std::string tz = time + ".000Z";
  tz[10] = 'T';
  return tz; 
}

oatpp::Object<CommentDto> CommentModel::createComment(std::string &userId, std::string &articleId, std::string &body, std::string &createTime) {
  try {
    // Insert comment
    Session session(Database::getPool()->get());
    session << "INSERT INTO comments (article_id, user_id, body, created_at, updated_at) VALUES (?, ?, ?, ?, ?)", use(articleId), use(userId), use(body), use(createTime), use(createTime), now;

    // Get id
    Poco::Nullable<unsigned int> retrunId;
    session << "SELECT last_insert_id()", into(retrunId), now;

    auto comment = CommentDto::createShared();
    comment->id = retrunId.value();
    comment->body = body;
    std::string tz = timeTz(createTime);
    comment->createdAt = tz;
    comment->updatedAt = tz;
    return comment;
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", exp.displayText().c_str());
    return nullptr;
  }
}