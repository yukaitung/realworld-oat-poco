#include "helper/DatabaseHelper.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"

#include "oatpp/core/base/Environment.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Exception;

std::shared_ptr<Poco::Data::SessionPool> DatabaseHelper::pool = nullptr;

void DatabaseHelper::initDatabase(const std::string &connectionName, const std::string &dbHost, const std::string &dbPort, const std::string &dbName, const std::string &dbUser, const std::string &dbPassword) {
  if(DatabaseHelper::pool != nullptr) {
    return;
  }
  Poco::Data::MySQL::Connector::registerConnector();
  std::string connectionString = "host=" + dbHost + ";port=" + dbPort + ";db=" + dbName + ";user=" + dbUser + ";password=" + dbPassword + ";compress=true;auto-reconnect=true";
  DatabaseHelper::pool = std::make_shared<Poco::Data::SessionPool>(connectionName, connectionString);
};

void DatabaseHelper::testTruncateDatabase() {
  try {
    Session session(DatabaseHelper::getSession());
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