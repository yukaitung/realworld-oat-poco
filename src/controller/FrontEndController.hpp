
#ifndef FRONTENDCONTROLLER_HPP
#define FRONTENDCONTROLLER_HPP

#include "dto/StatusDto.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "service/FrontEndService.hpp"
#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * User REST controller.
 */
class FrontEndController : public oatpp::web::server::api::ApiController {
private:
std::shared_ptr<FrontEndService> m_resources;

public:
  FrontEndController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    m_resources = FrontEndService::loadResources("front");
  }

  static std::shared_ptr<FrontEndController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<FrontEndController>(objectMapper);
  }

  ENDPOINT("GET", "/", getFrontend) {
    std::string ui = *m_resources->getResource("index.html");
    return createResponse(Status::CODE_200, ui);
  }

  ENDPOINT("GET", "/{filename}", getFrontendResource, PATH(String, filename)) {
    auto resp = createResponse(Status::CODE_200, m_resources->getResource(filename->c_str()));
    resp->putHeader("Content-Type", m_resources->getMimeType(filename));
    return resp;
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // FRONTENDCONTROLLER_HPP
