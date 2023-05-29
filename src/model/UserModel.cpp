#include "model/UserModel.hpp"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/Exception.h"
#include "Poco/SHA2Engine.h"

#include <random>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Exception;
using Poco::SHA2Engine;

UserModel::UserModel() {
  // TODO: use pool // dont insert secret
  connectionName = "MySQL";
  connectionString = "host=127.0.0.1;port=3306;db=Realworld;user=root;password=GueBPjHlPFUgXc7hm=;compress=true;auto-reconnect=true";
  Poco::Data::MySQL::Connector::registerConnector();
}

std::string UserModel::issueJWT(const std::string &username) {
  using Poco::JWT::Token;
  using Poco::JWT::Signer;

  Token token;
  token.setType("JWT");
  token.setSubject("REALWORLD");
  token.payload().set("name", username);
  token.setIssuedAt(Poco::Timestamp());

  Signer signer("REALWORLD-OAT-POCO-123456");
  std::string jwt = signer.sign(token, Signer::ALGO_HS256);
  return jwt;
}

oatpp::Object<UserDto> UserModel::createUser(std::string& email, std::string& username, std::string& password) {
  // Generate salt
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> random(65, 90);
  std::string salt;
  for(int i = 0; i < 8; i++)
    salt += char(random(gen));
  
  // Hash password
  password += salt;
  SHA2Engine sha256;
  sha256.update(password);
  password = sha256.digestToHex(sha256.digest());

  std::string token = issueJWT(username);

  auto user = UserDto::createShared();
  try 
  {
    Session session(connectionName, connectionString);
    Statement insert(session);
    insert << "INSERT INTO users (email, username, password, salt, token) VALUES (?, ?, ?, ?, ?)", use(email), use(username), use(password), use(salt), use(token);
    insert.execute();

    user->username = username;
    user->email = email;
    user->token = token;
    return user;
  }
  catch(Exception& exp)
  {
    std::cerr << exp.displayText() << std::endl;
    return user;
  }
}
