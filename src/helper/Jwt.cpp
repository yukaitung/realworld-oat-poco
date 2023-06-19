#include "helper/Jwt.hpp"

#include "Config.h"

#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"

using Poco::JWT::Token;
using Poco::JWT::Signer;
using Poco::Exception;

std::string Jwt::signerSecret = "";

void Jwt::setSignerSecret(const std::string &secret) {
  if(signerSecret.empty())
    signerSecret = secret;
}

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

  Signer signer(signerSecret);
  std::string jwt = signer.sign(token, Signer::ALGO_HS256);
  return jwt;
}

std::string Jwt::validateJWT(const std::string &jwt) {
  Signer signer(signerSecret);
  std::string id = "";

  try {
    Token token = signer.verify(jwt);
    Poco::LocalDateTime now;
    Poco::Timestamp tokenExpireTime = token.getExpiration();
    if(now.timestamp() > tokenExpireTime) {
      return "";
    }
    Poco::Dynamic::Var returnId = token.payload().get("id");
    id = returnId.toString(); // incorrect type --> exception
    return id;
  }
  catch(Exception& exp)
  {
    OATPP_LOGE("Jwt", ":%s(): %s", __func__, exp.displayText().c_str());
    return "";
  }
}