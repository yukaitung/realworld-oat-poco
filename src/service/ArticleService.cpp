#include "service/ArticleService.hpp"

#include "Poco/Timestamp.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"
#include "Poco/URI.h"

std::vector<std::string> ArticleService::splitStr(const std::string &s, const std::regex &sep_regex) {
  std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
  std::sregex_token_iterator end;
  return {iter, end};
}

oatpp::Object<ArticleJsonDto> ArticleService::createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto) {
  std::string title = dto->article->title;
  std::string description = dto->article->description;
  std::string body = dto->article->body;
  OATPP_ASSERT_HTTP(!title.empty(), Status::CODE_422, "The title is missing.");
  OATPP_ASSERT_HTTP(!description.empty(), Status::CODE_422, "The description is missing.");
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "The body is missing.");

  // Tags
  auto tags = dto->article->tagList;
  std::string tagsStr = "[";
  if(tags != nullptr) {
    bool result = tagModel.createTags(tags);
    OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

    // Generate JSON string
    auto tagsId = tagModel.getTagsIdFromNames(tags);
    for(int i = 0; i < tagsId->size(); i++) {
      tagsStr += tagsId->at(i);
      if(i < tagsId->size() - 1)
        tagsStr += ',';
    }
  }
  tagsStr += ']';
  
  // Slug, Create Time
  // slug = title replacing space + user id + timestamp
  Poco::LocalDateTime dateTime;
  std::string slug = "";
  Poco::URI::encode(title, "", slug);
  slug = slug + "-" + id + std::to_string(dateTime.timestamp().epochMicroseconds());
  std::string createTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());

  // Create article
  auto article = articleModel.createArticle(id, slug, title, description, body, tagsStr, createTime);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");

  // Sort tag name
  std::sort(tags->begin(), tags->end(), [] (const auto &lhs, const auto &rhs) {
    return lhs->compare(*rhs) < 0;
  });
  article->tagList = tags;

  // Author
  auto author = userModel.getProfileFromId(id);
  author->following = false;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::getArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing.");
  
  // Get article
  auto articleObj = articleModel.getArticle(slug);
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");
  auto articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(!authorId.empty(), Status::CODE_500, "Internal Server Error."); // Missing author id = error

  // Response data
  std::vector<std::string> articleIdList = {articleId};
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleIdList, id);
  OATPP_ASSERT_HTTP(favouriteData.size() > 0, Status::CODE_500, "Internal Server Error.");
  article->favourited = favouriteData[articleId].second;
  article->favouritesCount = favouriteData[articleId].first;
  auto author = userModel.getProfileFromId(authorId);
  if(!id.empty())
    author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  article->author = author;

  // Tag data
  std::string tagsListJson = std::get<ArticleModel::GetArticleEnum::TagsJsonStr>(articleObj);
  std::vector<std::string> tagsIdList = splitStr(tagsListJson, splitJsonArrRegex);
  tagsIdList.erase(tagsIdList.begin()); // First element is empty
  auto tagNameList = tagModel.getTagsName(tagsIdList);
  article->tagList = tagNameList;
  
  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticlesJsonDto> ArticleService::getArticles(std::string &id, unsigned int limit, unsigned int offset, std::string &tag, std::string &author, std::string &favouritedBy, bool feed) {
  // Convert User input to system parameter
  if(!tag.empty()) // tag name -> tag id
    tag = tagModel.getTagIdFromName(tag);
  if(!author.empty()) // author user name -> user Id
    author = userModel.getProfileFromUsername(author).second;
  if(!favouritedBy.empty())
    favouritedBy = userModel.getProfileFromUsername(favouritedBy).second;
  
  auto articlesObj = articleModel.getArticles(limit, offset, tag, author, favouritedBy, feed, id);

  // Append data for articles
  auto articles = std::get<ArticleModel::GetArticleEnum::Article>(articlesObj);
  OATPP_ASSERT_HTTP(articles != nullptr, Status::CODE_500, "Internal Server Error.");

  // Favourite
  std::vector<std::string> articleIdList = std::get<ArticleModel::GetArticleEnum::ArticleId>(articlesObj);
  std::unordered_map<std::string, std::pair<unsigned int, bool>> favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleIdList, id);

  // Get authors profile and following
  std::vector<std::string> tagsJsonStrList = std::get<ArticleModel::GetArticleEnum::TagsJsonStr>(articlesObj);
  std::vector<std::string> authorIdList = std::get<ArticleModel::GetArticleEnum::AuthorId>(articlesObj);
  std::unordered_map<std::string, oatpp::Object<UserProfileDto>> authorProfiles = userModel.getProfilesFromId(authorIdList);
  std::unordered_set<std::string> userFollingList;
  if(!id.empty()) {
    userFollingList = userHasFollowerModel.validUserIsFollowingFromList(id, authorIdList);
  }
  // Append data for each article
  for(int i = 0; i < articles->size(); i++) {
    // Tag data
    std::vector<std::string> tagsIdList = splitStr(tagsJsonStrList[i], splitJsonArrRegex);
    tagsIdList.erase(tagsIdList.begin()); // First element is empty
    articles->at(i)->tagList = tagModel.getTagsName(tagsIdList);

    // Favourite Count
    articles->at(i)->favouritesCount = favouriteData[articleIdList[i]].first;
    // Author Profile
    articles->at(i)->author = authorProfiles[authorIdList[i]];
    if(!id.empty()) {
      // User has favourtied the article
      articles->at(i)->favourited = favouriteData[articleIdList[i]].second;
      // Author following
      if(userFollingList.find(authorIdList[i]) != userFollingList.end())
        articles->at(i)->author->following = true;
    }
  }

  auto response = ArticlesJsonDto::createShared();
  response->articles = articles;
  response->articlesCount = articles->size();
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::updateArticle(std::string &id, std::string &slug, const oatpp::Object<ArticleExchangeJsonDto> &dto) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing.");
  
  // Get article, validate the author
  auto articleObj = articleModel.getArticle(slug);
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");
  auto articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");

  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(id.compare(authorId) == 0, Status::CODE_403, "The article is not belongs to the user.");

  std::string title = dto->article->title ? dto->article->title : "";
  std::string description = dto->article->description ? dto->article->description : "";
  std::string body = dto->article->body ? dto->article->body : "";

  // Tags
  auto tags = dto->article->tagList;
  std::string tagsStr = "";
  if(tags != nullptr) {
    tagsStr = "[";
    bool result = tagModel.createTags(tags);
    OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

    // Generate JSON string
    auto tagsId = tagModel.getTagsIdFromNames(tags);
    for(int i = 0; i < tagsId->size(); i++) {
      tagsStr += tagsId->at(i);
      if(i < tagsId->size() - 1)
        tagsStr += ',';
    }
    tagsStr += ']';
  }
  
  // New slug if title is changed
  Poco::LocalDateTime dateTime;
  std::string newSlug = "";
  if(!title.empty()) {
    newSlug = "";
    Poco::URI::encode(title, "", newSlug);
    newSlug = newSlug + "-" + id + std::to_string(dateTime.timestamp().epochMicroseconds());
  }
  std::string updateTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());

  if(!title.empty() || !description.empty() || !body.empty()) {
    bool result = articleModel.updateArticle(slug, newSlug, title, description, body, tagsStr, updateTime);
    OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");
  }
  
  // Update response data if necessary
  if(!title.empty()) {
    article->slug = newSlug;
    article->title = title;
  }
  if(!description.empty())
    article->description = description;
  if(!body.empty())
    article->body = body;
  article->updatedAt = articleModel.timeTz(updateTime);
  
  // Favourite, profile
  std::vector<std::string> articleIdList = {articleId};
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleIdList, id);
  OATPP_ASSERT_HTTP(favouriteData.size() > 0, Status::CODE_500, "Internal Server Error.");
  article->favourited = favouriteData[articleId].second;
  article->favouritesCount = favouriteData[articleId].first;
  auto author = userModel.getProfileFromId(id);
  article->author = author;

  // Tag data
  if(tagsStr.empty()) {
    // No update on tag, use old tag
    std::string tagsListJson = std::get<ArticleModel::GetArticleEnum::TagsJsonStr>(articleObj);
    std::vector<std::string> tagsIdList = splitStr(tagsListJson, splitJsonArrRegex);
    tagsIdList.erase(tagsIdList.begin()); // First element is empty
    article->tagList = tagModel.getTagsName(tagsIdList);
  }
  else {
    // Updated on tag, use old tag
    std::vector<std::string> tagsIdList = splitStr(tagsStr, splitJsonArrRegex);
    tagsIdList.erase(tagsIdList.begin()); // First element is empty
    article->tagList = tagModel.getTagsName(tagsIdList);
  }
  
  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::deleteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing");
  
  // Get article, validate the author
  auto articleObj = articleModel.getArticle(slug);
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(id.compare(authorId) == 0, Status::CODE_403, "The article is not belongs to the user.");

  bool result = articleModel.deleteArticle(slug);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  // Delete Comments
  result = commentModel.deleteCommentForArticle(articleId);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  // Delete Favourite
  result = articleHasFavouriteModel.deleteFavouriteForArticle(articleId);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  auto response = ArticleJsonDto::createShared();
  response->article = ArticleDto::createShared();
  response->article->slug = slug;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::favouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");

  // Favourite an article
  bool result = articleHasFavouriteModel.favouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  // Get favourite data
  std::vector<std::string> articleIdList = {articleId};
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleIdList, id);
  OATPP_ASSERT_HTTP(favouriteData.size() > 0, Status::CODE_500, "Internal Server Error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  article->favourited = true;
  article->favouritesCount = favouriteData[articleId].first;
  article->author = author;

  // Tag data
  std::string tagsListJson = std::get<ArticleModel::GetArticleEnum::TagsJsonStr>(articleObj);
  std::vector<std::string> tagsIdList = splitStr(tagsListJson, splitJsonArrRegex);
  tagsIdList.erase(tagsIdList.begin()); // First element is empty
  auto tagNameList = tagModel.getTagsName(tagsIdList);
  article->tagList = tagNameList;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::unfavouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Internal Server Error.");
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");

  // Unavourite an article
  bool result = articleHasFavouriteModel.unfavouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  // Get favourite data
  std::vector<std::string> articleIdList = {articleId};
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleIdList, id);
  OATPP_ASSERT_HTTP(favouriteData.size() > 0, Status::CODE_500, "Internal Server Error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  article->favourited = false;
  article->favouritesCount = favouriteData[articleId].first;
  article->author = author;

  // Tag data
  std::string tagsListJson = std::get<ArticleModel::GetArticleEnum::TagsJsonStr>(articleObj);
  std::vector<std::string> tagsIdList = splitStr(tagsListJson, splitJsonArrRegex);
  tagsIdList.erase(tagsIdList.begin()); // First element is empty
  auto tagNameList = tagModel.getTagsName(tagsIdList);
  article->tagList = tagNameList;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<CommentJsonDto> ArticleService::createComment(std::string &id, std::string &slug, const oatpp::Object<CommentJsonDto> &dto) {
  std::string body = dto->comment->body;
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "The comment is missing.");
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing.");

  std::string articleId = articleModel.getArticleIdFromSlug(slug);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");

  Poco::LocalDateTime dateTime;
  std::string createTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());
  auto comment = commentModel.createComment(id, articleId, body, createTime);
  OATPP_ASSERT_HTTP(comment, Status::CODE_500, "Internal Server Error.");

  auto response = CommentJsonDto::createShared();
  response->comment = comment;
  auto author = userModel.getProfileFromId(id);
  response->comment->author = author;

  return response;
}

oatpp::Object<CommentsJsonDto> ArticleService::getComments(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing.");

  std::string articleId = articleModel.getArticleIdFromSlug(slug);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "The article could not be found.");

  auto commentsData = commentModel.getComments(articleId);
  auto comments = commentsData.first;
  OATPP_ASSERT_HTTP(comments != nullptr, Status::CODE_500, "Internal Server Error.");
  std::vector<std::string> authorIds = commentsData.second;
  std::unordered_map<std::string, oatpp::Object<UserProfileDto>> authorProfiles = userModel.getProfilesFromId(authorIds);
  std::unordered_set<std::string> userFollingList;
  if(!id.empty()) {
    userFollingList = userHasFollowerModel.validUserIsFollowingFromList(id, authorIds);
  }
  // Append data for each article
  for(int i = 0; i < comments->size(); i++) {
    comments->at(i)->author = authorProfiles[authorIds[i]];
    if(!id.empty()) {
      if(userFollingList.find(authorIds[i]) != userFollingList.end())
        comments->at(i)->author->following = true;
    }
  }

  auto response = CommentsJsonDto::createShared();
  response->comments = comments;
  return response;
}

oatpp::Object<CommentJsonDto> ArticleService::deleteComment(std::string &id, std::string &slug, std::string &commentId) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "The slug is missing.");
  OATPP_ASSERT_HTTP(!commentId.empty(), Status::CODE_422, "The comment id is missing.");
  
  std::string authorId = commentModel.getCommentAuthorId(commentId);
  OATPP_ASSERT_HTTP(!authorId.empty(), Status::CODE_404, "The comment could not be found.");
  OATPP_ASSERT_HTTP(authorId.compare(id) == 0, Status::CODE_400, "The comment is not belongs to the user.");

  bool result = commentModel.deleteComment(commentId);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  auto response = CommentJsonDto::createShared();
  response->comment = CommentDto::createShared();
  response->comment->id = std::stoul(commentId);
  return response;
}

oatpp::Object<TagJsonDto> ArticleService::getTags() {
  auto tags = tagModel.getTags();
  OATPP_ASSERT_HTTP(tags, Status::CODE_500, "Internal Server Error.");
  return tags;
}