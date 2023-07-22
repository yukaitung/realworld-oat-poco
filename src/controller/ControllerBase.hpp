#ifndef CONTROLLERBASE_HPP
#define CONTROLLERBASE_HPP

#include "oatpp/web/server/api/ApiController.hpp"

class ControllerBase : public oatpp::web::server::api::ApiController {
  public:
    ControllerBase(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    }
    std::shared_ptr<OutgoingResponse> createCORSDtoResponse(const Status& status, const oatpp::Void& dto) const;
};

#endif // CONTROLLERBASE_HPP
