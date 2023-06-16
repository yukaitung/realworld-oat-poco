#include "model/UserHasFollowerModel.hpp"
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

bool UserHasFollowerModel::userHasThisFollower(std::string &userId, std::string &followerId) {
  Poco::Nullable<int> result;

  try {
    Session session(Database::getPool()->get());
    session << "SELECT COUNT(*) FROM users_has_followers WHERE user_id = ? AND follower_id = ?", into(result), use(userId), use(followerId), now;
    return (result == 1);
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool UserHasFollowerModel::userNewFollower(std::string &userId, std::string &followerId) {
  try {
    Session session(Database::getPool()->get());
    session << "INSERT INTO users_has_followers (user_id, follower_id) VALUES (?, ?)", use(userId), use(followerId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool UserHasFollowerModel::userRemoveFollower(std::string &userId, std::string &followerId) {
  try {
    Session session(Database::getPool()->get());
    session << "DELETE FROM users_has_followers WHERE user_id = ? AND follower_id = ?", use(userId), use(followerId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

std::unordered_set<std::string> UserHasFollowerModel::validUserIsFollowingFromList(std::string &followerId, std::vector<std::string> &userIds) {
  if(userIds.empty())
    return {};

  std::unordered_set<std::string> userFollowingIds;
  
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT user_id FROM users_has_followers WHERE user_id IN (";
    for(int i = 0; i < userIds.size(); i++) {
      select << "?", use(userIds[i]);
      if(i < userIds.size() - 1)
        select << ",";
    }
    select << ") AND follower_id = ?", use(followerId);
    select.execute();
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      std::string id = rs.value(0, i).toString();
      userFollowingIds.insert(id);
    }

    return userFollowingIds;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, ":%s(): %s", __func__, exp.displayText().c_str());
    return {};
  }
}