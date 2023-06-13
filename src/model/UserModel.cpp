#include "model/UserModel.hpp"
#include "helper/Jwt.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"
#include "Poco/SHA2Engine.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <random>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using Poco::SHA2Engine;
using oatpp::web::protocol::http::Status;

std::string UserModel::hashPassword(const std::string &passwordPlusSalt) {
  SHA2Engine sha256;
  sha256.update(passwordPlusSalt);
  return sha256.digestToHex(sha256.digest());
}

oatpp::Object<UserDto> UserModel::createUser(std::string &email, std::string &username, std::string &password) {
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

  try {
    // Insert user
    Session session(Database::getPool()->get());
    session << "INSERT INTO users (email, username, password, salt) VALUES (?, ?, ?, ?)", use(email), use(username), use(password), use(salt), now;

    // Get id
    Poco::Nullable<std::string> retrunId;
    session << "SELECT CAST(id AS char) FROM users WHERE email = ?", into(retrunId), use(email), now;

    // Update token
    std::string token = Jwt::issueJWT(retrunId.value());
    session << "UPDATE users SET token = ? WHERE (id = ?)", use(token), use(retrunId.value()), now;

    auto user = UserDto::createShared();
    user->username = username;
    user->email = email;
    user->token = token;
    return user;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

oatpp::Object<UserDto> UserModel::login(std::string &email, std::string &password) {
  Poco::Nullable<std::string> retrunId;
  Poco::Nullable<std::string> retrunUsername;
  Poco::Nullable<std::string> retrunEmail;
  Poco::Nullable<std::string> retrunPassword;
  Poco::Nullable<std::string> retrunSalt;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  // Fetch result
  try {
    Session session(Database::getPool()->get());
    session << "SELECT CAST(id AS char), username, email, password, salt, bio, image FROM users WHERE email = ?", into(retrunId), into(retrunUsername), into(retrunEmail), into(retrunPassword), into(retrunSalt), into(retrunBio), into(retrunImage), use(email), now;
    // Validate password
    if(!retrunPassword.isNull() && !retrunSalt.isNull()) {
      password += retrunSalt.value();
      std::string inputPassword = hashPassword(password);
      if(inputPassword.compare(retrunPassword.value()) != 0) {
        // Password incorrect
        OATPP_LOGE("UserModel", "User id %i unsuccessful login attempt.", retrunId.value().c_str());
        OATPP_ASSERT_HTTP(false, Status::CODE_403, "Unsuccessful login attempt.");
        return nullptr;
      }

      // Update token
      std::string token = Jwt::issueJWT(retrunId.value());
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
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

oatpp::Object<UserDto> UserModel::getUser(std::string &id) {
  Poco::Nullable<std::string> retrunUsername;
  Poco::Nullable<std::string> retrunEmail;
  Poco::Nullable<std::string> retrunToken;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT username, email, token, bio, image FROM users WHERE id = ?", into(retrunUsername), into(retrunEmail), into(retrunToken), into(retrunBio), into(retrunImage), use(id), now;

    auto user = UserDto::createShared();
    user->username = retrunUsername.value();
    user->email = retrunEmail.value();
    user->token = retrunToken.value();
    if(!retrunBio.isNull())
      user->bio = retrunBio.value();
    if(!retrunImage.isNull())
    user->image = retrunImage.value();
    return user;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

oatpp::Object<UserDto> UserModel::updateUser(std::string &id, std::string &email, std::string &username, std::string &password, std::string &bio, std::string &image) {
  try {
    Session session(Database::getPool()->get());
    Statement updateStatment(session);
    updateStatment << "UPDATE users SET";
    if(!email.empty())
      updateStatment << " email = ?,", use(email);
    if(!username.empty())
      updateStatment << " username = ?,", use(username);
    if(!password.empty())
      updateStatment << " password = ?,", use(password);
    if(!bio.empty())
      updateStatment << " bio = ?,", use(bio);
    if(!image.empty())
      updateStatment << " image = ?,", use(image);
    
    // Update token
    std::string token;
    token = Jwt::issueJWT(id);
    updateStatment << " token = ? WHERE id = ?", use(token), use(id);
    updateStatment.execute();

    // Obtain latest user data
    auto user = UserDto::createShared();
    user = getUser(id);
    return user;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

oatpp::Object<UserProfileDto> UserModel::getProfileFromId(std::string &id) {
  Poco::Nullable<std::string> retrunUsername;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT username, bio, image FROM users WHERE id = ?", into(retrunUsername), into(retrunBio), into(retrunImage), use(id), now;

    auto profile = UserProfileDto::createShared();
    profile->username = retrunUsername.value();
    if(!retrunBio.isNull())
      profile->bio = retrunBio.value();
    if(!retrunImage.isNull())
    profile->image = retrunImage.value();
    profile->following = false;
    return profile;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}

std::unordered_map<std::string, oatpp::Object<UserProfileDto>> UserModel::getProfilesFromId(std::vector<std::string> &ids) {
  std::unordered_map<std::string, oatpp::Object<UserProfileDto>> profiles;
  
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT id, username, bio, image FROM users WHERE id IN (";
    for(int i = 0; i < ids.size(); i++) {
      select << "?", use(ids[i]);
      if(i < ids.size() - 1)
        select << ",";
    }
    select << ")";
    select.execute();
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      std::string id = rs.value(0, i).toString();

      auto profile = UserProfileDto::createShared();
      profile->username = rs.value(1, i).toString();;
      if(!rs.value(2, i).isEmpty())
        profile->bio = rs.value(2, i).toString();
      if(!rs.value(3, i).isEmpty())
      profile->image = rs.value(3, i).toString();
      profile->following = false;

      profiles.insert({id, profile});
    }

    return profiles;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return {};
  }
}

std::pair<oatpp::Object<UserProfileDto>, std::string> UserModel::getProfileFromUsername(std::string &username) {
  Poco::Nullable<std::string> retrunId;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT CAST(id AS char), bio, image FROM users WHERE username = ?", into(retrunId), into(retrunBio), into(retrunImage), use(username), now;

    auto profile = UserProfileDto::createShared();
    profile->username = username;
    if(!retrunBio.isNull())
      profile->bio = retrunBio.value();
    if(!retrunImage.isNull())
    profile->image = retrunImage.value();
    profile->following = false;
    return {profile, retrunId.value()};
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return {nullptr, ""};
  }
}