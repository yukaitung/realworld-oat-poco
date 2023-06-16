#ifndef ARTICLEHASFAVOURITEMODEL_HPP
#define ARTICLEHASFAVOURITEMODEL_HPP

#include <string>
#include <utility>
#include <unordered_map>

class ArticleHasFavouriteModel {
  public:
    ArticleHasFavouriteModel() {};
    bool favouriteArticle(std::string &articleId, std::string &userId);
    bool unfavouriteArticle(std::string &articleId, std::string &userId);

    /**
    * This method is to obtain favourite count and see if the user favourited the article.
    * @param articleId The article id.
    * @param userId The article id.
    * @return std::pair<unsigned int, bool> This returns the favorite count and the user's favourited status for the article. The favourite count becomes -1 if an error occured.
    */
    std::pair<unsigned int, bool> getArticlefavouriteData(std::string &articleId, std::string &userId);
    
    /**
    * This method is to obtain favourite count and see if the user favourited the article.
    * @param articleId The list of article id.
    * @param userId The list of article id.
    * @return std::unordered_map<std::string, std::pair<unsigned int, bool>>  This returns the favorite count and the user's favourited status for the article. The favourite count becomes -1 if an error occured.
    */
    std::unordered_map<std::string, std::pair<unsigned int, bool>> getArticlesfavouriteData(std::vector<std::string> &articleId, std::string &userId);
    std::vector<std::string> getUserFavourite(std::string &userId);
};

#endif // ARTICLEHASFAVOURITEMODEL_HPP