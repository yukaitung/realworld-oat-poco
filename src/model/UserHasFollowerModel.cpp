#include "model/UserHasFollowerModel.hpp"
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

bool UserHasFollowerModel::userNewFollower(std::string &userId, std::string &followerId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "INSERT INTO users_has_followers (user_id, follower_id) VALUES (?, ?)", use(userId), use(followerId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

bool UserHasFollowerModel::userRemoveFollower(std::string &userId, std::string &followerId) {
  try {
    Session session(DatabaseHelper::getSession());
    session << "DELETE FROM users_has_followers WHERE user_id = ? AND follower_id = ?", use(userId), use(followerId), now;
    return true;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return false;
  }
}

std::unordered_set<std::string> UserHasFollowerModel::validUserFollowing(std::string &userId, std::vector<std::string> &followingId) {
  if(followingId.empty())
    return {};

  std::unordered_set<std::string> userFollowingIds;
  
  try {
    Session session(DatabaseHelper::getSession());
    Statement select(session);
    select << "SELECT user_id FROM users_has_followers WHERE user_id IN (";
    for(int i = 0; i < followingId.size(); i++) {
      select << "?", use(followingId[i]);
      if(i < followingId.size() - 1)
        select << ",";
    }
    select << ") AND follower_id = ?", use(userId);
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
    OATPP_LOGE("UserHasFollowerModel", ":%s(): %s", __func__, exp.displayText().c_str());
    return {};
  }
}