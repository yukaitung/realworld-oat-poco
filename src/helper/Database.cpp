#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Exception;

std::shared_ptr<Poco::Data::SessionPool> Database::pool = nullptr;

void Database::InitDatabase(std::string connectionName, std::string connectionString) {
  if(Database::pool != nullptr) {
    return;
  }
  Poco::Data::MySQL::Connector::registerConnector();
  Database::pool = std::make_shared<Poco::Data::SessionPool>(connectionName, connectionString);
};

void Database::TestTruncateDatabase() {
  try {
    Session session(Database::getPool()->get());
    session << "TRUNCATE articles", now;
    session << "TRUNCATE articles_has_favourites", now;
    session << "TRUNCATE comments", now;
    session << "TRUNCATE tags", now;
    session << "TRUNCATE users", now;
    session << "TRUNCATE users_has_followers", now;
  }
  catch(Exception& exp) {
    OATPP_LOGE("DatabaseHelper", ":%s(): %s", __func__, exp.displayText().c_str());
  }
}