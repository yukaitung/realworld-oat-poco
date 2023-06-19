#include "ArticleControllerTest.hpp"
#include "TestData.hpp"
#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include "controller/ArticleController.hpp"
#include "controller/UserController.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp-test/web/ClientServerTestRunner.hpp"

#include <cstdio>

void ArticleControllerTest::onRun() {
  /* Register test components */
  TestComponent component;

  /* Create client-server test runner */
  oatpp::test::web::ClientServerTestRunner runner;

  /* Add UserController endpoints to the router of the test server */
  runner.addController(UserController::createShared());
  runner.addController(ArticleController::createShared());
  runner.addController(ArticleControllerOptionalAuth::createShared());

  /* Run test */
  runner.run([this, &runner] {
    /* Get client connection provider for Api Client */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider);

    /* Get object mapper component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    /* Create http request executor for Api Client */
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);

    /* Create Test API client */
    auto client = TestClient::createShared(requestExecutor, objectMapper);

    // Start Test
    OATPP_LOGD("ArticleController", "Login");
    auto loginDto = UserAuthJsonDto::createShared();
    loginDto->user = UserAuthDto::createShared();
    loginDto->user->email = TestData::user[0].email;
    loginDto->user->password = TestData::user[0].password;
    auto response = client->login(loginDto);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto loginResponseDto = response->readBodyToDto<oatpp::Object<UserJsonDto>>(objectMapper.get());
    OATPP_ASSERT(loginResponseDto->user != nullptr);
    OATPP_ASSERT(loginResponseDto->user->username != nullptr && loginResponseDto->user->username->compare(TestData::user[0].username) == 0);
    OATPP_ASSERT(loginResponseDto->user->email != nullptr && loginResponseDto->user->email->compare(TestData::user[0].email) == 0);
    OATPP_ASSERT(loginResponseDto->user->token != nullptr && !loginResponseDto->user->token->empty());
    userToken = "Token " + loginResponseDto->user->token;

    OATPP_LOGD("ArticleController", "Create article");
    auto articleDto = ArticleExchangeJsonDto::createShared();
    articleDto->article = ArticleExchangeDto::createShared();
    articleDto->article->title = TestData::article[0].title;
    articleDto->article->description = TestData::article[0].description;
    articleDto->article->body = TestData::article[0].body;
    articleDto->article->tagList = oatpp::Vector<oatpp::String>::createShared();
    for(int i = 0; i < TestData::article[0].tagId.size(); i++) {
      articleDto->article->tagList->push_back(TestData::tag[TestData::article[0].tagId[i]]);
    }
    response = client->createArticle(articleDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && !articleResponseDto->article->slug->empty());
    currentSlug = articleResponseDto->article->slug;
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Get an article by slug");
    response = client->getArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) == 0);
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Favourite an article");
    response = client->favouriteArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) == 0);
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == true);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 1);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Validate the article is favouried");
    response = client->getArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) == 0);
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == true);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 1);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Unfavourite an article");
    response = client->unfavouriteArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) == 0);
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Validate the article is unfavouried");
    response = client->getArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) == 0);
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[0].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[0].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[0].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[0].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Create comments in an article");
    auto commentDto = CommentJsonDto::createShared();
    commentDto->comment = CommentDto::createShared();
    commentDto->comment->body = TestData::comment[0];
    response = client->createComment(currentSlug, commentDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto commentResponseDto = response->readBodyToDto<oatpp::Object<CommentJsonDto>>(objectMapper.get());
    OATPP_ASSERT(commentResponseDto != nullptr);
    OATPP_ASSERT(commentResponseDto->comment->id != nullptr);
    currentCommentId = commentResponseDto->comment->id;
    OATPP_ASSERT(commentResponseDto->comment->createdAt != nullptr && !commentResponseDto->comment->createdAt->empty());
    OATPP_ASSERT(commentResponseDto->comment->updatedAt != nullptr && !commentResponseDto->comment->updatedAt->empty());
    OATPP_ASSERT(commentResponseDto->comment->body != nullptr && commentResponseDto->comment->body->compare(TestData::comment[0]) == 0);
    OATPP_ASSERT(commentResponseDto->comment->author != nullptr);
    OATPP_ASSERT(commentResponseDto->comment->author->username != nullptr && commentResponseDto->comment->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Get comments from an article");
    response = client->getComments(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto commentsResponseDto = response->readBodyToDto<oatpp::Object<CommentsJsonDto>>(objectMapper.get());
    OATPP_ASSERT(commentsResponseDto != nullptr);
    OATPP_ASSERT(commentsResponseDto->comments != nullptr && commentsResponseDto->comments->size() == 1);
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->id == currentCommentId);
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->createdAt != nullptr && !commentsResponseDto->comments->at(0)->createdAt->empty());
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->updatedAt != nullptr && !commentsResponseDto->comments->at(0)->updatedAt->empty());
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->body != nullptr && commentsResponseDto->comments->at(0)->body->compare(TestData::comment[0]) == 0);
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->author != nullptr);
    OATPP_ASSERT(commentsResponseDto->comments->at(0)->author->username != nullptr && commentsResponseDto->comments->at(0)->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Delete comment from an article");
    response = client->deleteComment(currentSlug, currentCommentId, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);

    OATPP_LOGD("ArticleController", "Validate the comments is deleted");
    response = client->getComments(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    commentsResponseDto = response->readBodyToDto<oatpp::Object<CommentsJsonDto>>(objectMapper.get());
    OATPP_ASSERT(commentsResponseDto != nullptr);
    OATPP_ASSERT(commentsResponseDto->comments != nullptr && commentsResponseDto->comments->empty());

    OATPP_LOGD("ArticleController", "Update an article");
    articleDto->article->title = TestData::article[1].title;
    articleDto->article->description = TestData::article[1].description;
    articleDto->article->body = TestData::article[1].body;
    articleDto->article->tagList->clear();
    for(int i = 0; i < TestData::article[1].tagId.size(); i++) {
      articleDto->article->tagList->push_back(TestData::tag[TestData::article[1].tagId[i]]);
    }
    response = client->updateArticle(currentSlug, articleDto, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && articleResponseDto->article->slug->compare(currentSlug) != 0);
    currentSlug = articleResponseDto->article->slug;
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[1].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[1].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[1].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[1].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);

    OATPP_LOGD("ArticleController", "Validate the article is updated");
    response = client->getArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articleResponseDto != nullptr);
    OATPP_ASSERT(articleResponseDto->article->slug != nullptr && !articleResponseDto->article->slug->empty());
    OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[1].title) == 0);
    OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[1].description) == 0);
    OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[1].body) == 0);
    OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[1].tagId.size());
    OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
    OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
    OATPP_ASSERT(articleResponseDto->article->favourited == false);
    OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
    OATPP_ASSERT(articleResponseDto->article->author != nullptr);
    OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);
    
    OATPP_LOGD("ArticleController", "Delete an article");
    response = client->deleteArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);

    OATPP_LOGD("ArticleController", "Validate the article is deleted");
    response = client->getArticle(currentSlug, userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 404);

    for(int i = 0; i < TestData::article.size(); i++) {
      OATPP_LOGD("ArticleController", "Create %d of %d articles", i + 1, TestData::article.size());
      auto articleDto = ArticleExchangeJsonDto::createShared();
      articleDto->article = ArticleExchangeDto::createShared();
      articleDto->article->title = TestData::article[i].title;
      articleDto->article->description = TestData::article[i].description;
      articleDto->article->body = TestData::article[i].body;
      articleDto->article->tagList = oatpp::Vector<oatpp::String>::createShared();
      for(int j = 0; j < TestData::article[i].tagId.size(); j++) {
        articleDto->article->tagList->push_back(TestData::tag[TestData::article[i].tagId[j]]);
      }
      response = client->createArticle(articleDto, userToken);
      OATPP_ASSERT(response != nullptr);
      OATPP_ASSERT(response->getStatusCode() == 200);
      auto articleResponseDto = response->readBodyToDto<oatpp::Object<ArticleJsonDto>>(objectMapper.get());
      OATPP_ASSERT(articleResponseDto != nullptr);
      OATPP_ASSERT(articleResponseDto->article->slug != nullptr && !articleResponseDto->article->slug->empty());
      OATPP_ASSERT(articleResponseDto->article->title != nullptr && articleResponseDto->article->title->compare(TestData::article[i].title) == 0);
      OATPP_ASSERT(articleResponseDto->article->description != nullptr && articleResponseDto->article->description->compare(TestData::article[i].description) == 0);
      OATPP_ASSERT(articleResponseDto->article->body != nullptr && articleResponseDto->article->body->compare(TestData::article[i].body) == 0);
      OATPP_ASSERT(articleResponseDto->article->tagList != nullptr && articleResponseDto->article->tagList->size() == TestData::article[i].tagId.size());
      OATPP_ASSERT(articleResponseDto->article->createdAt != nullptr && !articleResponseDto->article->createdAt->empty());
      OATPP_ASSERT(articleResponseDto->article->updatedAt != nullptr && !articleResponseDto->article->updatedAt->empty());
      OATPP_ASSERT(articleResponseDto->article->favourited == false);
      OATPP_ASSERT(articleResponseDto->article->favouritesCount == 0);
      OATPP_ASSERT(articleResponseDto->article->author != nullptr);
      OATPP_ASSERT(articleResponseDto->article->author->username != nullptr && articleResponseDto->article->author->username->compare(TestData::user[0].username) == 0);
    }

    OATPP_LOGD("ArticleController", "Get all articles");
    response = client->getArticles("", "", "", userToken);
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto articlesResponseDto = response->readBodyToDto<oatpp::Object<ArticlesJsonDto>>(objectMapper.get());
    OATPP_ASSERT(articlesResponseDto != nullptr);
    OATPP_ASSERT(articlesResponseDto->articlesCount == TestData::article.size());
    OATPP_ASSERT(articlesResponseDto->articles->size() == TestData::article.size());
    for(int i = 0; i < TestData::article.size(); i++) {
      OATPP_LOGD("ArticleController", "Validate %d of %d articles", i + 1, TestData::article.size());
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->slug != nullptr && !articlesResponseDto->articles->at(i)->slug->empty());
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->title != nullptr && articlesResponseDto->articles->at(i)->title->compare(TestData::article[i].title) == 0);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->description != nullptr && articlesResponseDto->articles->at(i)->description->compare(TestData::article[i].description) == 0);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->body != nullptr && articlesResponseDto->articles->at(i)->body->compare(TestData::article[i].body) == 0);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->tagList != nullptr && articlesResponseDto->articles->at(i)->tagList->size() == TestData::article[i].tagId.size());
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->createdAt != nullptr && !articlesResponseDto->articles->at(i)->createdAt->empty());
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->updatedAt != nullptr && !articlesResponseDto->articles->at(i)->updatedAt->empty());
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->favourited == false);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->favouritesCount == 0);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->author != nullptr);
      OATPP_ASSERT(articlesResponseDto->articles->at(i)->author->username != nullptr && articlesResponseDto->articles->at(i)->author->username->compare(TestData::user[0].username) == 0);
    }

    OATPP_LOGD("ArticleController", "Get all tags");
    response = client->getTags();
    OATPP_ASSERT(response != nullptr);
    OATPP_ASSERT(response->getStatusCode() == 200);
    auto tagResponseDto = response->readBodyToDto<oatpp::Object<TagJsonDto>>(objectMapper.get());
    OATPP_ASSERT(tagResponseDto != nullptr);
    OATPP_ASSERT(tagResponseDto->tags != nullptr && tagResponseDto->tags->size() == TestData::tag.size());
  }, std::chrono::minutes(2) /* test timeout */);

  /* wait all server threads finished */
  std::this_thread::sleep_for(std::chrono::seconds(1));
}