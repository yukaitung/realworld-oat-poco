#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <map>
#include <vector>
#include <string>

class TagModel {
  private:
    static std::map<std::string, std::string> tagCache;

  public:
    TagModel() {};
    static void initCache();
    bool createTags(std::vector<std::string> tags);
    std::vector<std::string> getTagsId(std::vector<std::string> tags);
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP