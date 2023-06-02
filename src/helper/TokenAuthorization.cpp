#include "helper/TokenAuthorization.hpp"
#include "helper/Jwt.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

using namespace oatpp::web::server::handler;
using oatpp::web::protocol::http::Status;

std::shared_ptr<AuthorizationObject> TokenAuthorizationHandler::handleAuthorization(const oatpp::String &header) {
  if(header && header->size() > 6 && oatpp::utils::String::compare(header->data(), 6, "Token ", 6) == 0) {

    oatpp::String token = oatpp::String(header->c_str() + 6, header->size() - 6);

    auto authResult = authorize(token);
    if(authResult) {
      return authResult;
    }

    OATPP_ASSERT_HTTP(false, Status::CODE_401, "Unauthorized");
  }

  OATPP_ASSERT_HTTP(false, Status::CODE_401, "Authorization Required");
}

std::shared_ptr<AuthorizationObject> TokenAuthorizationHandler::authorize(const oatpp::String& token) {
  if(token != "") {
    std::string id = Jwt::validateJWT(token);
    OATPP_ASSERT_HTTP(!id.empty(), Status::CODE_401, "Token Error");
    return std::make_shared<TokenAuthorizationObject>(id);
  }
  OATPP_ASSERT_HTTP(false, Status::CODE_401, "Authorization Failed");
  return nullptr;
}