#ifndef ARTICLEMODEL_HPP
#define ARTICLEMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>
#include <utility>

class ArticleModel {
  public:
    enum GetArticleEnum {Article, ArticleId, AuthorId, TagsJsonStr};
    
    ArticleModel() {};
    
    /**
    * This method is to create an article into database.
    * @param userId The author user id of article.
    * @param slug The slug of article.
    * @param title The title of article.
    * @param description The description of article.
    * @param tags The tags of article.
    * @param body The content of article.
    * @param createTime The create time and update time of article.
    * @return oatpp::Object<ArticleDto> This returns article object. Remember to append data for author profile.
    */
    oatpp::Object<ArticleDto> createArticle(std::string &userId, std::string &slug, std::string &title, std::string &description, std::string &body, std::string &tagsStr, std::string &createTime);
    
    /**
    * This method is to get an article by slug.
    * @param slug The slug of article.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> This returns article object, article id, author user id, tags json in string. Remember to append data for favourite, taglist and author profile.
    */
    std::tuple<oatpp::Vector<oatpp::Object<ArticleDto>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> getArticleFromSlug(std::string &slug) {
      static std::string dummy = "";
      return getArticleFunction(slug, 1, 0, dummy, dummy, dummy, false, dummy);
    }

    /**
    * This method is to fetch a list of articles from database.
    * @param limit The maximun number of article.
    * @param offset The offset of article.
    * @param tagId The article has this tag id.
    * @param authorId The author of the article.
    * @param favouritedBy The article is favourited by user id.
    * @param feed This is feed endpoind.
    * @param userId Feed for user id.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> This returns article object, article id, author user id, tags json in string. Remember to append data for favourite, taglist and author profile.
    */
    std::tuple<oatpp::Vector<oatpp::Object<ArticleDto>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> getArticleFromCondition(unsigned int limit, unsigned int offset, std::string &tagId, std::string &authorId, std::string &favouritedBy, bool feed, std::string &userId) {
      static std::string dummy = "";
      return getArticleFunction(dummy, limit, offset, tagId, authorId, favouritedBy, feed, userId);
    }

    /**
    * This method is to fetch a list of articles from database.
    * @param slug The slug of article.
    * @param limit The maximun number of article.
    * @param offset The offset of article.
    * @param tagId The article has this tag id.
    * @param authorId The author of the article.
    * @param favouritedBy The article is favourited by user id.
    * @param feed This is feed endpoind.
    * @param userId Feed for user id.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> This returns article object, article id, author user id, tags json in string. Remember to append data for favourite, taglist and author profile.
    */
    std::tuple<oatpp::Vector<oatpp::Object<ArticleDto>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> getArticleFunction(std::string &slug, unsigned int limit, unsigned int offset, std::string &tagId, std::string &authorId, std::string &favouritedBy, bool feed, std::string &userId);
    
    /**
    * This method is to update an article.
    * @param slug The current slug for the article
    * @param newSlug The new slug for article
    * @param title The new title for article
    * @param description The new description for article
    * @param body The new body for article
    * @param tagsStr The new tag json for article
    * @param updateTime The latest update time
    * @return bool This return the result
    */
    bool updateArticle(std::string &slug, std::string &newSlug, std::string &title, std::string &description, std::string &body, std::string &tagsStr, std::string &updateTime);

    /**
    * This method is to delete an article.
    * @param slug The current slug for the article
    * @return bool This return the result
    */
    bool deleteArticle(std::string &slug);

    /**
    * This method is to obtain article id from slug
    * @param slug The current slug for the article
    * @return std::string This return the article id
    */
    std::string getArticleIdFromSlug(std::string &slug);
};

#endif // ARTICLEMODEL_HPP