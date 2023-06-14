#include "service/ArticleService.hpp"

#include "Poco/Timestamp.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"

std::string &ArticleService::removeHeadSpace(std::string &s, const char *t) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

std::string &ArticleService::removeTailSpace(std::string &s, const char *t) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

std::string &ArticleService::removeBothSpace(std::string &s, const char *t){
  return removeHeadSpace(removeTailSpace(s, t), t);
}

std::string ArticleService::replaceSpace(std::string &s) {
  std::regex r("\\s");
  s = std::regex_replace(s, r, "-");
  return s;
}

std::vector<std::string> ArticleService::splitStr(const std::string &s, const std::regex &sep_regex) {
  std::sregex_token_iterator iter(s.begin(), s.end(), sep_regex, -1);
  std::sregex_token_iterator end;
  return {iter, end};
}

oatpp::Object<ArticleJsonDto> ArticleService::createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto) {
  std::string title = dto->article->title;
  title = removeBothSpace(title);
  std::string description = dto->article->description;
  std::string body = dto->article->body;
  OATPP_ASSERT_HTTP(!title.empty(), Status::CODE_422, "Missing title.");
  OATPP_ASSERT_HTTP(!description.empty(), Status::CODE_422, "Missing description.");
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "Missing body.");

  // Create tags
  auto tags = dto->article->tagList;
  std::string tagsStr = "[";
  if(tags != nullptr) {
    // Copy oatpp object to std object
    std::vector<std::string> tagsStd(tags->size());
    for(int i = 0; i < tags->size(); i++) {
      tagsStd[i] = tags->at(i);
    }
    bool result = tagModel.createTags(tagsStd);
    OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

    // Generate JSON string
    auto tagsId = tagModel.getTagsId(tagsStd);
    for(int i = 0; i < tagsId->size(); i++) {
      tagsStr += tagsId->at(i);
      if(i < tagsId->size() - 1)
        tagsStr += ',';
    }
  }
  tagsStr += ']';
  
  // slug = title replacing space + user id + timestamp
  Poco::LocalDateTime dateTime;
  std::string slug = title;
  slug = replaceSpace(slug) + "-" + id + std::to_string(dateTime.timestamp().epochMicroseconds());
  std::string createTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());

  // Create article
  auto article = articleModel.createArticle(id, slug, title, description, body, tagsStr, createTime);
  OATPP_ASSERT_HTTP(article != nullptr, Status::CODE_500, "Server error.");
  article->tagList = tags;

  // Response data
  auto author = userModel.getProfileFromId(id);
  author->following = false;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::getArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");
  
  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(!authorId.empty(), Status::CODE_500, "Server error."); // Missing author id = error

  // Response data
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  auto articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");
  article->favourited = favouriteData.second;
  article->favouritesCount = favouriteData.first;
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
    tag = tagModel.getTagId(tag);
  if(!author.empty()) // author user name -> user Id
    author = userModel.getProfileFromUsername(author).second;
  if(!favouritedBy.empty())
    favouritedBy = userModel.getProfileFromUsername(favouritedBy).second;
  
  auto articlesObj = articleModel.getArticles(limit, offset, tag, author, favouritedBy, feed, id);

  // Response data
  auto articles = std::get<ArticleModel::GetArticleEnum::Article>(articlesObj);
  OATPP_ASSERT_HTTP(articles != nullptr, Status::CODE_500, "Server error.");

  // Favourite
  std::vector<std::string> articleIdList = std::get<ArticleModel::GetArticleEnum::ArticleId>(articlesObj);
  std::unordered_map<std::string, std::pair<unsigned int, bool>> favouriteData = articleHasFavouriteModel.getArticlefavouriteDataFromList(articleIdList, id);

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
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");
  
  // Get article, validate the author
  auto articleObj = articleModel.getArticle(slug);
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(id.compare(authorId) == 0, Status::CODE_400, "Unauthorized access.");

  std::string title = dto->article->title ? dto->article->title : "";
  title = removeBothSpace(title);
  std::string description = dto->article->description ? dto->article->description : "";
  std::string body = dto->article->body ? dto->article->body : "";

  // New slug if title is changed
  Poco::LocalDateTime dateTime;
  std::string newSlug = "";
  if(!title.empty()) {
    newSlug = title;
    newSlug = replaceSpace(newSlug) + "-" + id + std::to_string(dateTime.timestamp().epochMicroseconds());
  }
  std::string updateTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());

  if(!title.empty() || !description.empty() || !body.empty()) {
    bool result = articleModel.updateArticle(slug, newSlug, title, description, body, updateTime);
    OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");
  }

  // Response data
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  auto articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);

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
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");
  article->favourited = favouriteData.second;
  article->favouritesCount = favouriteData.first;
  auto author = userModel.getProfileFromId(id);
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

oatpp::Object<ArticleJsonDto> ArticleService::deleteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");
  
  // Get article, validate the author
  auto articleObj = articleModel.getArticle(slug);
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  OATPP_ASSERT_HTTP(id.compare(authorId) == 0, Status::CODE_400, "Unauthorized access.");

  bool result = articleModel.deleteArticle(slug);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  auto response = ArticleJsonDto::createShared();
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::favouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_500, "Server error."); // Missing ArticleId = error

  // Favourite an article
  bool result = articleHasFavouriteModel.favouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  // Get favourite data
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  article->favourited = true;
  article->favouritesCount = favouriteData.first;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<ArticleJsonDto> ArticleService::unfavouriteArticle(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");

  // Get article
  auto articleObj = articleModel.getArticle(slug);
  std::string articleId = std::get<ArticleModel::GetArticleEnum::ArticleId>(articleObj);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_500, "Server error."); // Missing ArticleId = error

  // Unavourite an article
  bool result = articleHasFavouriteModel.unfavouriteArticle(articleId, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  // Get favourite data
  auto favouriteData = articleHasFavouriteModel.getArticlefavouriteData(articleId, id);
  OATPP_ASSERT_HTTP(favouriteData.first >= 0, Status::CODE_500, "Server error.");

  // Response data
  std::string authorId = std::get<ArticleModel::GetArticleEnum::AuthorId>(articleObj);
  auto author = userModel.getProfileFromId(authorId);
  author->following = userHasFollowerModel.userHasThisFollower(authorId, id);
  
  auto article = std::get<ArticleModel::GetArticleEnum::Article>(articleObj);
  article->favourited = false;
  article->favouritesCount = favouriteData.first;
  article->author = author;

  auto response = ArticleJsonDto::createShared();
  response->article = article;
  return response;
}

oatpp::Object<CommentJsonDto> ArticleService::createComment(std::string &id, std::string &slug, const oatpp::Object<CommentJsonDto> &dto) {
  std::string body = dto->comment->body;
  OATPP_ASSERT_HTTP(!body.empty(), Status::CODE_422, "Missing body.");
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "Missing slug.");

  std::string articleId = articleModel.getArticleIdFromSlug(slug);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "Article not found.");

  Poco::LocalDateTime dateTime;
  std::string createTime = Poco::DateTimeFormatter::format(dateTime.timestamp(), "%Y-%m-%d %H:%M:%S", Poco::Timezone::tzd());
  auto comment = commentModel.createComment(id, articleId, body, createTime);
  OATPP_ASSERT_HTTP(comment, Status::CODE_500, "Server error.");

  auto response = CommentJsonDto::createShared();
  response->comment = comment;
  auto author = userModel.getProfileFromId(id);
  response->comment->author = author;

  return response;
}

oatpp::Object<CommentsJsonDto> ArticleService::getComments(std::string &id, std::string &slug) {
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_422, "Missing slug.");

  std::string articleId = articleModel.getArticleIdFromSlug(slug);
  OATPP_ASSERT_HTTP(!articleId.empty(), Status::CODE_404, "Article not found.");

  auto commentsData = commentModel.getComments(articleId);
  auto comments = commentsData.first;
  OATPP_ASSERT_HTTP(comments != nullptr, Status::CODE_500, "Server Error.");
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
  OATPP_ASSERT_HTTP(!slug.empty(), Status::CODE_400, "Missing slug");
  OATPP_ASSERT_HTTP(!commentId.empty(), Status::CODE_400, "Missing comment id");
  
  std::string authorId = commentModel.getCommentAuthorId(commentId);
  OATPP_ASSERT_HTTP(!authorId.empty(), Status::CODE_404, "Comment Not Found.");
  OATPP_ASSERT_HTTP(authorId.compare(id) == 0, Status::CODE_400, "Unauthorized access.");

  bool result = commentModel.deleteComment(commentId);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server error.");

  auto response = CommentJsonDto::createShared();
  response->comment = CommentDto::createShared();
  response->comment->id = std::stoul(commentId);
  return response;

}

oatpp::Object<TagJsonDto> ArticleService::getTags() {
  auto tags = tagModel.getTags();
  OATPP_ASSERT_HTTP(tags, Status::CODE_500, "Server error.");
  return tags;
}