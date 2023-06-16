#include "helper/Jwt.hpp"

#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"

using Poco::JWT::Token;
using Poco::JWT::Signer;
using Poco::Exception;

std::string Jwt::issueJWT(const std::string &id) {
  Token token;
  token.setType("JWT");
  token.setSubject("REALWORLD");
  token.payload().set("id", id);
  
  // The expires in 15 minutes
  Poco::LocalDateTime time;
  token.setIssuedAt(time.timestamp());
  time += Poco::Timespan(0, 0, 15, 0, 0);
  token.setExpiration(time.timestamp());

  Signer signer("REALWORLD-OAT-POCO-123456");
  std::string jwt = signer.sign(token, Signer::ALGO_HS256);
  return jwt;
}

std::string Jwt::validateJWT(const std::string &jwt) {
  Signer signer("REALWORLD-OAT-POCO-123456");

  std::string id = "";
  try {
    Token token = signer.verify(jwt);
    // TODO: expire
    Poco::Dynamic::Var returnId = token.payload().get("id");
    id = returnId.toString(); // incorrect type --> exception
    return id;
  }
  catch(Exception& exp)
  {
    OATPP_LOGE("Jwt", ":%s(): %s", __func__, exp.displayText().c_str());
    return id;
  }
}