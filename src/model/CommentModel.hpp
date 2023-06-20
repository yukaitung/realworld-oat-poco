#ifndef COMMENTMODEL_HPP
#define COMMENTMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <string>
#include <utility>

class CommentModel {
  public:
    CommentModel() {};
    
    /**
    * This method is to create a comment for an article.
    * @param userId The user id
    * @param articleId The article id being commented
    * @param body The comment content
    * @param createTime The create time
    * @return oatpp::Object<CommentDto> This return the comment object
    */
    oatpp::Object<CommentDto> createComment(std::string &userId, std::string &articleId, std::string &body, std::string &createTime);
    
    /**
    * This method is to get all comments in article
    * @param articleId The article id
    * @return std::pair<oatpp::Vector<oatpp::Object<CommentDto>>, std::vector<std::string>> This return the list of comments and author user id
    */
    std::pair<oatpp::Vector<oatpp::Object<CommentDto>>, std::vector<std::string>> getComments(std::string &articleId);
    
    /**
    * This method is to delete an comment.
    * @param commentId The comment id 
    * @return bool This return the result
    */
    bool deleteComment(std::string &commentId);
    
    /**
    * This method is to delete all comments in an article.
    * @param articleId The article id being commented
    * @return bool This return the result
    */
    bool deleteCommentForArticle(std::string &articleId);
    
    /**
    * This method is to get comment author id from comment id
    * @param commentId The comment id 
    * @return std::string This return the author id
    */
    std::string getAuthorIdFromCommentId(std::string &commentId);
};

#endif // COMMENTMODEL_HPP