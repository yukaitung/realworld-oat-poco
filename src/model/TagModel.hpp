#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <map>
#include <string>

class TagModel {
  private:
    static std::map<std::string, std::string> tagCache;

  public:
    TagModel() {};
    static void initCache();
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP