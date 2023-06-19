#ifndef ARTICLECONTROLLER_HPP
#define ARTICLECONTROLLER_HPP

#include "service/ArticleService.hpp"
#include "helper/TokenAuthorization.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "Poco/URI.h"

#include <cstring>

#include OATPP_CODEGEN_BEGIN(ApiController)

/**
 * Article REST controller.
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

  ENDPOINT_INFO(feedArticles) {
    info->summary = "Get multiple articles created by followed users, ordered by most recent first";
    info->addResponse<Object<ArticlesJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("GET", "/articles/feed", feedArticles, QUERIES(QueryParams, queryParams), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    unsigned int limit = 20;
    unsigned int offset = 0;
    std::string dummy = "";
    for(auto &param : queryParams.getAll()) {
      const char *key = static_cast<const char*>(param.first.getData());
      const char *value = static_cast<const char*>(param.second.getData());
      if(strcmp(key, "limit") == 0)
        limit = std::stoul(value);
      else if(strcmp(key, "offset") == 0)
        offset = std::stoul(value);
    }
    return createDtoResponse(Status::CODE_200, articleService.getArticles(authObject->id, limit, offset, dummy, dummy, dummy, true));
  }

  ENDPOINT_INFO(createArticle) {
    info->summary = "Create new article";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("POST", "/articles", createArticle, BODY_DTO(Object<ArticleExchangeJsonDto>, dto), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    return createDtoResponse(Status::CODE_200, articleService.createArticle(authObject->id, dto));
  }

  ENDPOINT_INFO(updateArticle) {
    info->summary = "Update an article";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_403, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("PUT", "/articles/{slug}", updateArticle, PATH(String, slug), BODY_DTO(Object<ArticleExchangeJsonDto>, dto), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.updateArticle(authObject->id, slugStr, dto));
  }

  ENDPOINT_INFO(deleteArticle) {
    info->summary = "Delete an article";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_403, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("DELETE", "/articles/{slug}", deleteArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.deleteArticle(authObject->id, slugStr));
  }

  ENDPOINT_INFO(favouriteArticle) {
    info->summary = "Favourite an article";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("POST", "/articles/{slug}/favourite", favouriteArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.favouriteArticle(authObject->id, slugStr));
  }

  ENDPOINT_INFO(unfavouriteArticle) {
    info->summary = "Unfavourite an article";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("DELETE", "/articles/{slug}/favourite", unfavouriteArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.unfavouriteArticle(authObject->id, slugStr));
  }

  ENDPOINT_INFO(createComment) {
    info->summary = "Create a comment in the article";
    info->addResponse<Object<CommentJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("POST", "/articles/{slug}/comments", createComment, PATH(String, slug), BODY_DTO(Object<CommentJsonDto>, dto), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.createComment(authObject->id, slugStr, dto));
  }

  ENDPOINT_INFO(deleteComment) {
    info->summary = "Delete a comment in the article";
    info->addResponse<Object<CommentJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("DELETE", "/articles/{slug}/comments/{id}", deleteComment, PATH(String, slug), PATH(String, id), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    std::string commentIdStr = id.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.deleteComment(authObject->id, slugStr, commentIdStr));
  }

  ENDPOINT_INFO(getTags) {
    info->summary = "Get all tags";
    info->addResponse<Object<TagJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addTag("ArticleController");
  }
  ENDPOINT("GET", "/tags", getTags)
  {
    return createDtoResponse(Status::CODE_200, articleService.getTags());
  }
};

/**
 * Article REST controller, the authentication is optional.
 */
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

  ENDPOINT_INFO(getArticle) {
    info->summary = "Get an article (Authorization is optional)";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("GET", "/articles/{slug}", getArticle, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.getArticle(authObject->id, slugStr));
  }

  ENDPOINT_INFO(getArticles) {
    info->summary = "Get a list of articles (Authorization is optional)";
    info->addResponse<Object<ArticlesJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("GET", "/articles", getArticles, QUERIES(QueryParams, queryParams), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string tag = "";
    std::string author = "";
    std::string favouritedBy = "";
    unsigned int limit = 20;
    unsigned int offset = 0;
    for(auto &param : queryParams.getAll()) {
      const char *key = static_cast<const char*>(param.first.getData());
      const char *value = static_cast<const char*>(param.second.getData());
      if(strcmp(key, "tag") == 0) 
        Poco::URI::decode(value, tag);
      else if(strcmp(key, "author") == 0)
        Poco::URI::decode(value, author);
      else if(strcmp(key, "favourited") == 0)
        Poco::URI::decode(value, favouritedBy);
      else if(strcmp(key, "limit") == 0)
        limit = std::stoul(value);
      else if(strcmp(key, "offset") == 0)
        offset = std::stoul(value);
    }
    return createDtoResponse(Status::CODE_200, articleService.getArticles(authObject->id, limit, offset, tag, author, favouritedBy));
  }

  ENDPOINT_INFO(getComments) {
    info->summary = "Get comments for an article (Authorization is optional)";
    info->addResponse<Object<ArticleJsonDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_401, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_422, "application/json");
    info->addResponse<Object<ErrorJsonDto>>(Status::CODE_500, "application/json");
    info->addSecurityRequirement("Token");
    info->addTag("ArticleController");
  }
  ENDPOINT("GET", "/articles/{slug}/comments", getComments, PATH(String, slug), AUTHORIZATION(std::shared_ptr<TokenAuthorizationObject>, authObject))
  {
    std::string slugStr = slug.getValue("");
    return createDtoResponse(Status::CODE_200, articleService.getComments(authObject->id, slugStr));
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif // ARTICLECONTROLLER_HPP
