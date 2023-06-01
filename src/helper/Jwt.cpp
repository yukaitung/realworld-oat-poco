#include "helper/Jwt.hpp"

#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"

using Poco::JWT::Token;
using Poco::JWT::Signer;
using Poco::Exception;

std::string Jwt::issueJWT(const std::string &username) {
  Token token;
  token.setType("JWT");
  token.setSubject("REALWORLD");
  token.payload().set("name", username);
  token.setIssuedAt(Poco::Timestamp());

  Signer signer("REALWORLD-OAT-POCO-123456");
  std::string jwt = signer.sign(token, Signer::ALGO_HS256);
  return jwt;
}

std::string Jwt::validateJWT(const std::string &jwt) {
  Signer signer("REALWORLD-OAT-POCO-123456");

  std::string username = "";
  try {
    Token token = signer.verify(jwt);
    Poco::Dynamic::Var name = token.payload().get("name");
    username = name.toString(); // incorrect type --> exception
    return username;
  }
  catch(Exception& exp)
  {
    OATPP_LOGE("Jwt", exp.displayText().c_str());
    return username;
  }
}