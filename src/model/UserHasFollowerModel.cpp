#include "model/UserHasFollowerModel.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

bool UserHasFollowerModel::userHasThisFollower(std::string &userId, std::string &followerId) {
  Poco::Nullable<int> result;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT COUNT(*) FROM users_has_followers WHERE user_id = ? AND follower_id = ?", into(result), use(userId), use(followerId), now;

    return (result == 1);
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserHasFollowerModel", exp.displayText().c_str());
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
    OATPP_LOGE("UserHasFollowerModel", exp.displayText().c_str());
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
    OATPP_LOGE("UserHasFollowerModel", exp.displayText().c_str());
    return false;
  }
}