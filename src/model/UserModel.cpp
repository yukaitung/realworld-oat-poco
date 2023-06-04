#include "model/UserModel.hpp"
#include "helper/Jwt.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "Poco/SHA2Engine.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <random>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
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
    updateStatment << " token = ? WHERE id = ?", use(token), use(id), now;
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

oatpp::Object<AuthorDto> UserModel::getAuthor(std::string &id) {
  Poco::Nullable<std::string> retrunUsername;
  Poco::Nullable<std::string> retrunBio;
  Poco::Nullable<std::string> retrunImage;

  try {
    // Fetch result
    Session session(Database::getPool()->get());
    session << "SELECT username, bio, image FROM users WHERE id = ?", into(retrunUsername), into(retrunBio), into(retrunImage), use(id), now;

    auto author = AuthorDto::createShared();
    author->username = retrunUsername.value();
    if(!retrunBio.isNull())
      author->bio = retrunBio.value();
    if(!retrunImage.isNull())
    author->image = retrunImage.value();
    return author;
  }
  catch(Exception& exp) {
    OATPP_LOGE("UserModel", exp.displayText().c_str());
    return nullptr;
  }
}