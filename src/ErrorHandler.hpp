
#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include "dto/StatusDto.hpp"

#include "oatpp/web/server/handler/ErrorHandler.hpp"
#include "oatpp/web/protocol/http/outgoing/ResponseFactory.hpp"

class ErrorHandler : public oatpp::web::server::handler::ErrorHandler {
private:
  typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
  typedef oatpp::web::protocol::http::Status Status;
  typedef oatpp::web::protocol::http::outgoing::ResponseFactory ResponseFactory;
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> objMapper;

public:
  ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper);
  std::shared_ptr<OutgoingResponse> handleError(const Status &status, const oatpp::String &message, const Headers &headers) override;
};

#endif // ERRORHANDLER_HPP