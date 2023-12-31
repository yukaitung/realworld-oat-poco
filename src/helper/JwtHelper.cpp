#include "helper/JwtHelper.hpp"

#include "Config.h"

#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"

using Poco::JWT::Token;
using Poco::JWT::Signer;
using Poco::Exception;

std::string JwtHelper::signerSecret = "";

void JwtHelper::setSignerSecret(const std::string &secret) {
  if(signerSecret.empty())
    signerSecret = secret;
}

std::string JwtHelper::issueJWT(const std::string &id) {
  Token token;
  token.setType("JWT");
  token.setSubject("REALWORLD");
  token.payload().set("id", id);
  
  // The expires in 15 minutes
  Poco::Timestamp time;
  token.setIssuedAt(time);
  time += Poco::Timespan(0, 0, 30, 0, 0);
  token.setExpiration(time);

  Signer signer(signerSecret);
  std::string jwt = signer.sign(token, Signer::ALGO_HS256);
  return jwt;
}

std::string JwtHelper::validateJWT(const std::string &jwt) {
  Signer signer(signerSecret);
  std::string id = "";

  try {
    Token token = signer.verify(jwt);
    Poco::Timestamp now;
    Poco::Timestamp tokenExpireTime = token.getExpiration();
    if(now > tokenExpireTime) {
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