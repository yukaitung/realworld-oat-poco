
#ifndef ARTICLECONTROLLER_HPP
#define ARTICLECONTROLLER_HPP

#include "service/ArticleService.hpp"
#include "helper/TokenAuthorization.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * User REST controller.
 */
class ArticleController : public oatpp::web::server::api::ApiController {
private:
  ArticleService articleService;

public:
  ArticleController(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<TokenAuthorizationHandler>());
  }

  static std::shared_ptr<ArticleController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ArticleController>(objectMapper);
  }

  ENDPOINT("POST", "/articles", createArticle, BODY_DTO(Object<ArticleExchangeJsonDto>, dto), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    return createDtoResponse(Status::CODE_200, articleService.createArticle(authObject->id, dto));
  }

  ENDPOINT("POST", "/articles/{slug}/favorite", favouriteArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.favouriteArticle(authObject->id, slugStr));
  }

  ENDPOINT("DELETE", "/articles/{slug}/favorite", unfavouriteArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.unfavouriteArticle(authObject->id, slugStr));
  }
};

class ArticleControllerOptionalAuth : public oatpp::web::server::api::ApiController {
private:
  ArticleService articleService;

public:
  ArticleControllerOptionalAuth(const std::shared_ptr<ObjectMapper>& objectMapper) : oatpp::web::server::api::ApiController(objectMapper) {
    setDefaultAuthorizationHandler(std::make_shared<OptionalTokenAuthorizationHandler>());
  }

  static std::shared_ptr<ArticleControllerOptionalAuth> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::make_shared<ArticleControllerOptionalAuth>(objectMapper);
  }

  ENDPOINT("GET", "/articles/{slug}", getArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.getArticle(authObject->id, slugStr));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // ARTICLECONTROLLER_HPP
