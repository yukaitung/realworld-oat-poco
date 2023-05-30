#include "model/UserModel.hpp"

#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/Exception.h"
#include "Poco/SHA2Engine.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <random>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Exception;
using Poco::SHA2Engine;
using oatpp::web::protocol::http::Status;

UserModel::UserModel() {
  // TODO: use pool // dont insert secret
  connectionName = "MySQL";
  connectionString = "host=127.0.0.1;port=3306;db=Realworld;user=root;password=GueBPjHlPFUgXc7hm=;compress=true;auto-reconnect=true";
  Poco::Data::MySQL::Connector::registerConnector();
}

std::string UserModel::hashPassword(const std::string &passwordPlusSalt) {
  SHA2Engine sha256;
  sha256.update(passwordPlusSalt);
  return sha256.digestToHex(sha256.digest());
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
  password = hashPassword(password);

  std::string token = issueJWT(username);

  try 
  {
    Session session(connectionName, connectionString);
    session << "INSERT INTO users (email, username, password, salt, token) VALUES (?, ?, ?, ?, ?)", use(email), use(username), use(password), use(salt), use(token), now;

    auto user = UserDto::createShared();
    user->username = username;
    user->email = email;
    user->token = token;
    return user;
  }
  catch(Exception& exp)
  {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

oatpp::Object<UserDto> UserModel::login(std::string &email, std::string &password) {
  Poco::Nullable<int> retrunId;
  Poco::Nullable<std::string> retrunUsername;
  Poco::Nullable<std::string> retrunEmail;
  Poco::Nullable<std::string> retrunPassword;
  Poco::Nullable<std::string> retrunSalt;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  // Fetch result
  try 
  {
    Session session(connectionName, connectionString);
    session << "SELECT id, username, email, password, salt, bio, image FROM users WHERE email = ?", into(retrunId), into(retrunUsername), into(retrunEmail), into(retrunPassword), into(retrunSalt), into(retrunBio), into(retrunImage), use(email), now;
    // Validate password
    if(!retrunPassword.isNull() && !retrunSalt.isNull()) {
      password += retrunSalt.value();
      std::string inputPassword = hashPassword(password);
      if(inputPassword.compare(retrunPassword.value()) != 0) {
        // Password incorrect
        OATPP_LOGE("UserModel", "User id %i unsuccessful login attempt.", retrunId.value());
        OATPP_ASSERT_HTTP(false, Status::CODE_403, "Unsuccessful login attempt.");
        return nullptr;
      }

      // Update token
      std::string token = issueJWT(retrunUsername.value());
      session << "UPDATE users SET token = ? WHERE (id = ?)", use(token), use(retrunId.value()), now;

      auto user = UserDto::createShared();
      user->username = retrunUsername.value();
      user->email = retrunEmail.value();
      user->token = token;
      if(!retrunBio.isNull())
        user->bio = retrunBio.value();
      if(!retrunImage.isNull())
      user->image = retrunImage.value();
      return user;
    }
    
    // Error in fetching data
    return nullptr;
  }
  catch(Exception& exp)
  {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}