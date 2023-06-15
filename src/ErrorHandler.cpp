#include "ErrorHandler.hpp"

ErrorHandler::ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper) : objMapper(objectMapper) {}

std::shared_ptr<ErrorHandler::OutgoingResponse> ErrorHandler::handleError(const Status &status, const oatpp::String &message, const Headers &headers) {
  auto error = ErrorJsonDto::createShared();
  error->errors = ErrorBodyDto::createShared();
  error->errors->body = oatpp::Vector<oatpp::String>::createShared();
  error->errors->body->push_back(message);

  auto response = ResponseFactory::createResponse(status, error, objMapper);

  for(const auto& pair : headers.getAll()) {
    response->putHeader(pair.first.toString(), pair.second.toString());
  }

  return response;

}