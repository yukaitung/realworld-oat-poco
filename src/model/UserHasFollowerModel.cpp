#include "model/UserHasFollowerModel.hpp"
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

bool UserHasFollowerModel::userHasThisFollower(std::string userId, std::string followerId) {
  Poco::Nullable<int> result;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT COUNT(*) FROM users_has_followers WHERE user_id = ? AND follower_id = ?;", into(result), use(userId), use(followerId), now;

    return (result == 1);
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return false;
  }
}