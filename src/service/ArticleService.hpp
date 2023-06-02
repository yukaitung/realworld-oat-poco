#ifndef ARTICLESERVICE_HPP
#define ARTICLESERVICE_HPP

#include "dto/ArticleDto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

class ArticleService {
private:
  typedef oatpp::web::protocol::http::Status Status;

public:
  oatpp::Object<ArticleExchangeDto> createArticle(std::string &id, const oatpp::Object<ArticleExchangeJsonDto> &dto);
};

#endif // ARTICLESERVICE_HPP