#include "model/CommentModel.hpp"
#include "helper/CommonHelper.hpp"
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

oatpp::Object<CommentDto> CommentModel::createComment(std::string &userId, std::string &articleId, std::string &body, std::string &createTime) {
  try {
    // Insert comment
    Session session(DatabaseHelper::getSession());
    session << "INSERT INTO comments (article_id, user_id, body, created_at, updated_at) VALUES (?, ?, ?, ?, ?)", use(articleId), use(userId), use(body), use(createTime), use(createTime), now;

    // Get id
    Poco::Nullable<unsigned int> retrunId;
    session << "SELECT last_insert_id()", into(retrunId), now;

    auto comment = CommentDto::createShared();
    comment->id = retrunId.value();
    comment->body = body;
    std::string tz = CommonHelper::timeTz(createTime);
    comment->createdAt = tz;
    comment->updatedAt = tz;
    return comment;
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return nullptr;
  }
}

std::pair<oatpp::Vector<oatpp::Object<CommentDto>>, std::vector<std::string>> CommentModel::getComments(std::string &articleId) {
  try {
    // Insert comment
    Session session(DatabaseHelper::getSession());
    Statement select(session);
    select << "SELECT id, CAST(user_id AS char), body, created_at, updated_at FROM comments WHERE article_id = ? ORDER BY updated_at DESC, id DESC", use(articleId), now;
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    oatpp::Vector<oatpp::Object<CommentDto>> comments = oatpp::Vector<oatpp::Object<CommentDto>>::createShared();
    comments->resize(rowCount);
    std::vector<std::string> authorIds(rowCount);

    for(int i = 0; i < rowCount; i++) {
      auto comment = CommentDto::createShared();
      comment->id = rs.value(0, i).convert<unsigned int>();
      comment->body = rs.value(2, i).toString();
      std::string createdAt = rs.value(3, i).toString();
      std::string updatedAt = rs.value(4, i).toString();
      comment->createdAt = CommonHelper::timeTz(createdAt);
      comment->updatedAt = CommonHelper::timeTz(updatedAt);
      comments->at(i) = comment;
      authorIds[i] = rs.value(1, i).toString();
    }

    return {comments, authorIds};
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {nullptr, {}};
  }
}

std::string CommentModel::getAuthorIdFromCommentId(std::string &commentId) {
  Poco::Nullable<std::string> retrunId;

  try {
    Session session(DatabaseHelper::getSession());
    session << "SELECT CAST(user_id AS char) FROM comments WHERE id = ?", use(commentId), into(retrunId), now;
    return !retrunId.isNull() ? retrunId.value() : "";
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return "";
  }
}

bool CommentModel::deleteComment(std::string &commentId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "DELETE FROM comments WHERE id = ?", use(commentId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool CommentModel::deleteCommentForArticle(std::string &articleId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "DELETE FROM comments WHERE article_id = ?", use(articleId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("CommentModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}