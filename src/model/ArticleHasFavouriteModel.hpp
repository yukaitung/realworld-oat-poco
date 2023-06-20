#ifndef ARTICLEHASFAVOURITEMODEL_HPP
#define ARTICLEHASFAVOURITEMODEL_HPP

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

class ArticleHasFavouriteModel {
  public:
    ArticleHasFavouriteModel() {};

    /**
    * This method is to create a favourite record.
    * @param articleId The article id.
    * @param userId The article id.
    * @return bool This return the result
    */
    bool favouriteArticle(std::string &articleId, std::string &userId);

    /**
    * This method is to remove a favourite record.
    * @param articleId The article id.
    * @param userId The article id.
    * @return bool This return the result
    */
    bool unfavouriteArticle(std::string &articleId, std::string &userId);

    /**
    * This method is to remove all favourite record for an article.
    * @param articleId The article id.
    * @return bool This return the result
    */
    bool deleteFavouriteForArticle(std::string &articleId);

    /**
    * This method is to obtain favourite count and user's favourited status.
    * @param articleId The list of article id.
    * @param userId The list of article id.
    * @return std::unordered_map<std::string, std::pair<unsigned int, bool>> This returns a list of favourite count and the user's favourited status for the article.
    */
    std::unordered_map<std::string, std::pair<unsigned int, bool>> getArticlefavouriteData(std::vector<std::string> &articleId, std::string &userId);
    
    /**
    * This method is to obtain all user favourited articles.
    * @param userId The list of article id.
    * @return std::vector<std::string> This returns a list of article id favourited by user.
    */
    std::vector<std::string> getUserFavourite(std::string &userId);
};

#endif // ARTICLEHASFAVOURITEMODEL_HPP