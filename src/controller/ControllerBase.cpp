#include "controller/ControllerBase.hpp"

std::shared_ptr<ControllerBase::OutgoingResponse> ControllerBase::createCORSDtoResponse(const Status& status, const oatpp::Void& dto) const {
  std::shared_ptr<ControllerBase::OutgoingResponse> resp = ResponseFactory::createResponse(status, dto, m_defaultObjectMapper);
  resp->putHeaderIfNotExists(oatpp::web::protocol::http::Header::CORS_ORIGIN, "*"); \
  resp->putHeaderIfNotExists(oatpp::web::protocol::http::Header::CORS_METHODS, "GET, POST, PUT, DELETE, OPTIONS"); \
  resp->putHeaderIfNotExists(oatpp::web::protocol::http::Header::CORS_HEADERS, "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");\
  resp->putHeaderIfNotExists(oatpp::web::protocol::http::Header::CORS_MAX_AGE, "1728000");
  resp->putHeaderIfNotExists("Access-Control-Allow-Credentials", "true");
  return resp;
}