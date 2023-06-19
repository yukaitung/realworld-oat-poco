#ifndef ARTICLEMODEL_HPP
#define ARTICLEMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>
#include <utility>

class ArticleModel {
  public:
    enum GetArticleEnum {Article, ArticleId, AuthorId, TagsJsonStr};
    
    ArticleModel() {};
    std::string timeTz(std::string &time);
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
    * This method is to fetch an article from database.
    * @param slug The slug of article.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> This returns article object, article id, author user id, tags json in string. Remember to append data for favourite, taglist and author profile.
    */
    std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> getArticle(std::string &slug);
    /**
    * This method is to fetch a list of articles from database.
    * @return std::tuple<oatpp::Object<ArticleDto>, std::string, std::string, std::string> This returns article object, article id, author user id, tags json in string. Remember to append data for favourite, taglist and author profile.
    */
    std::tuple<oatpp::Vector<oatpp::Object<ArticleDto>>, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> getArticles(unsigned int limit, unsigned int offset, std::string &tagId, std::string &authorId, std::string &favouritedBy, bool feed, std::string &userId);
    bool updateArticle(std::string &slug, std::string &newSlug, std::string &title, std::string &description, std::string &body, std::string &tagsStr, std::string &updateTime);
    bool deleteArticle(std::string &slug);
    std::string getArticleIdFromSlug(std::string &slug);
};

#endif // ARTICLEMODEL_HPP