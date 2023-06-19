#ifndef TESTCLIENT_HPP
#define TESTCLIENT_HPP

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/UserDto.hpp"
#include "dto/ArticleDto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class TestClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(TestClient)

  // User Controller
  API_CALL("POST", "/users", createUser, BODY_DTO(Object<UserRegJsonDto>, dto))
  API_CALL("POST", "/users/login", login, BODY_DTO(Object<UserAuthJsonDto>, dto))
  API_CALL("GET", "/user", getUser, HEADER(String, token, "Authorization"))
  API_CALL("PUT", "/user", updateUser, BODY_DTO(Object<UserUpdateJsonDto>, dto), HEADER(String, token, "Authorization"))

  // Article Controller
  API_CALL("GET", "/articles/feed", feedArticles, HEADER(String, token, "Authorization"))
  API_CALL("POST", "/articles", createArticle, BODY_DTO(Object<ArticleExchangeJsonDto>, dto), HEADER(String, token, "Authorization"))
  API_CALL("GET", "/articles/{slug}", getArticle, PATH(String, slug), HEADER(String, token, "Authorization"))
  API_CALL("GET", "/articles", getArticles, QUERY(String, tag), QUERY(String, author), QUERY(String, favourited), HEADER(String, token, "Authorization"))
  API_CALL("PUT", "/articles/{slug}", updateArticle, PATH(String, slug), BODY_DTO(Object<ArticleExchangeJsonDto>, dto), HEADER(String, token, "Authorization"))
  API_CALL("DELETE", "/articles/{slug}", deleteArticle, PATH(String, slug), HEADER(String, token, "Authorization"))
  API_CALL("POST", "/articles/{slug}/favourite", favouriteArticle, PATH(String, slug), HEADER(String, token, "Authorization"))
  API_CALL("DELETE", "/articles/{slug}/favourite", unfavouriteArticle, PATH(String, slug), HEADER(String, token, "Authorization"))
  API_CALL("POST", "/articles/{slug}/comments", createComment, PATH(String, slug), BODY_DTO(Object<CommentJsonDto>, dto), HEADER(String, token, "Authorization"))
  API_CALL("GET", "/articles/{slug}/comments", getComments, PATH(String, slug), HEADER(String, token, "Authorization"))
  API_CALL("DELETE", "/articles/{slug}/comments/{id}", deleteComment, PATH(String, slug), PATH(UInt32, id), HEADER(String, token, "Authorization"))
  API_CALL("GET", "/tags", getTags)

  // Profile Controller
  API_CALL("GET", "/profiles/{username}", getProfile, PATH(String, username), HEADER(String, token, "Authorization"))
  API_CALL("POST", "/profiles/{username}/follow", followProfile, PATH(String, username), HEADER(String, token, "Authorization"))
  API_CALL("DELETE", "/profiles/{username}/follow", unfollowProfile, PATH(String, username), HEADER(String, token, "Authorization"))
};

#include OATPP_CODEGEN_END(ApiClient)

#endif // TESTCLIENT_HPP