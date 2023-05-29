
#ifndef USERCONTROLLER_HPP
#define USERCONTROLLER_HPP

#include "service/UserService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * User REST controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
private:
  UserService userService;

public:
  UserController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
  }

  static std::shared_ptr<UserController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<UserController>(objectMapper);
  }

  ENDPOINT("POST", "/users", createUser, BODY_DTO(Object<UserRegJsonDto>, dto))
  {
    return createDtoResponse(Status::CODE_200, userService.createUser(dto));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // USERCONTROLLER_HPP
