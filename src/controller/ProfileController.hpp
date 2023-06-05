
#ifndef PROFILECONTROLLER_HPP
#define PROFILECONTROLLER_HPP

#include "service/ProfileService.hpp"
#include "helper/TokenAuthorization.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * Profile REST controller.
 */
class ProfileController : public oatpp::web::server::api::ApiController {
private:

public:
  ProfileController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<TokenAuthorizationHandler>());
  }

  static std::shared_ptr<ProfileController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ProfileController>(objectMapper);
  }
};

/**
 * Profile REST controller, the authentication is optional.
 */
class ProfileControllerOptionalAuth : public oatpp::web::server::api::ApiController {
private:
  ProfileService profileService;

public:
  ProfileControllerOptionalAuth(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<OptionalTokenAuthorizationHandler>());
  }

  static std::shared_ptr<ProfileControllerOptionalAuth> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ProfileControllerOptionalAuth>(objectMapper);
  }

  ENDPOINT("GET", "/profiles/{username}", getProfile, PATH(String, username), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string usernameStr = username.getValue("");
    return createDtoResponse(Status::CODE_200, profileService.getProfile(authObject->id, usernameStr));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // PROFILECONTROLLER_HPP
