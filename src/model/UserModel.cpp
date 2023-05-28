#include "model/UserModel.hpp"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"
#include "Poco/SHA2Engine.h"

#include <random>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Exception;
using Poco::SHA2Engine;

UserModel::UserModel()
{
  // todo: use pool // dont insert secret
  connectionName = "MySQL";
  connectionString = "host=127.0.0.1;port=3306;db=Realworld;user=root;password=GueBPjHlPFUgXc7hm=;compress=true;auto-reconnect=true";
  Poco::Data::MySQL::Connector::registerConnector();
}

bool UserModel::createUser(std::string& email, std::string& username, std::string& password)
{

  // Generate salt
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> random(65, 90);
  string salt;
  for(int i = 0; i < 8; i++)
    salt += char(random(gen));
  
  // Hash password
  password += salt;
  SHA2Engine sha256;
  sha256.update(password);
  password = sha256.digestToHex(sha256.digest());

  try 
  {
    Session session(connectionName, connectionString);
    Statement insert(session);
    insert << "INSERT INTO users (email, username, password, salt) VALUES (?, ?, ?, ?)", use(email), use(username), use(password), use(salt);
    insert.execute();
    return true;
  }
  catch(Exception& exp)
  {
    std::cerr << exp.displayText() << std::endl;
    return false;
  }
}
