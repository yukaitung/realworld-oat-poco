
#ifndef FRONTENDCONTROLLER_HPP
#define FRONTENDCONTROLLER_HPP

#include "dto/StatusDto.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

// Reuse oatpp-swagger
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/web/protocol/http/outgoing/StreamingBody.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * User REST controller.
 */
class FrontEndController : public oatpp::web::server::api::ApiController {
private:
std::shared_ptr<oatpp::swagger::Resources> m_resources;

public:
  FrontEndController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    m_resources = std::make_shared<oatpp::swagger::Resources>("front");
  }

  static std::shared_ptr<FrontEndController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<FrontEndController>(objectMapper);
  }

  bool hasEnding(std::string fullString, std::string const &ending) const {
    std::transform(fullString.begin(), fullString.end(), fullString.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
  };

  std::string getMimeType(const std::string &filename) const {
    if (hasEnding(filename, ".html")) return "text/html";
    if (hasEnding(filename, ".jpg")) return "image/jpeg";
    if (hasEnding(filename, ".jpeg")) return "image/jpeg";
    if (hasEnding(filename, ".png")) return "image/png";
    if (hasEnding(filename, ".gif")) return "image/gif";
    if (hasEnding(filename, ".css")) return "text/css";
    if (hasEnding(filename, ".js")) return "text/javascript";
    if (hasEnding(filename, ".xml")) return "text/xml";
    return "text/plain";
  };

  ENDPOINT("GET", "/", getFrontend) {
    std::string ui;

      v_char8 buffer[1024];
      auto fileStream = m_resources->getResourceStream("index.html");
      oatpp::data::stream::BufferOutputStream s(1024);
      oatpp::data::stream::transfer(fileStream, &s, 0, buffer, 1024);
      ui = s.toString();

    return createResponse(Status::CODE_200, ui);
  }

  ENDPOINT("GET", "/{filename}", getFrontendResource, PATH(String, filename)) {
 
      auto body = std::make_shared<oatpp::web::protocol::http::outgoing::StreamingBody>(
        m_resources->getResourceStream(filename->c_str())
      );
      auto resp = OutgoingResponse::createShared(Status::CODE_200, body);
      resp->putHeader("Content-Type", getMimeType(filename));
      return resp;

  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // FRONTENDCONTROLLER_HPP
