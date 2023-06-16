#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iterator>

class TagCache {
  private:
    std::map<std::string, const std::string*> tagNamePairTagIdMap;
    std::map<std::string, const std::string*> tagIdPairTagNameMap;

  public:
    TagCache() {};
    void createTag(const std::pair<std::string, std::string> &tagNamePairTagId);
    std::string getIdFromName(const std::string &tagName);
    std::string getNameFromId(const std::string &id);
    bool nameExist(const std::string name); 
    const std::map<std::string, const std::string*>::iterator getTagNameBegin() {return tagNamePairTagIdMap.begin();};
    const std::map<std::string, const std::string*>::iterator getTagNameEnd() {return tagNamePairTagIdMap.end();};
    const size_t getSize() {return tagNamePairTagIdMap.size();};
};

class TagModel {
  private:
    static TagCache tagCache;

  public:
    TagModel() {};

    /**
    * This method is to initialize the cache when the program is stating.
    */
    static void InitCache();

    /**
    * This method is to create a list of tags.
    * @param tags The list of of tags.
    * @return bool This returns the result.
    */
    bool createTags(const oatpp::Vector<oatpp::String> &tags);

    /**
    * This method is to get tag id from tags cache.
    * @param tag The tag name.
    * @return std::string This returns the tag id.
    */
    std::string getTagIdFromName(const std::string &tag);

    /**
    * This method is to get a list of tag id from tags cache.
    * @param tags The list of tag name.
    * @return oatpp::Vector<oatpp::String> This returns the list of tag id.
    */
    oatpp::Vector<oatpp::String> getTagsIdFromNames(const oatpp::Vector<oatpp::String> &tags);

    oatpp::Vector<oatpp::String> getTagsName(const std::vector<std::string> &tagsId);
    oatpp::Object<TagJsonDto> getTags();
};

#endif // TAGMODEL_HPP