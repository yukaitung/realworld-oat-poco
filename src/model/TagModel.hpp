#ifndef TAGMODEL_HPP
#define TAGMODEL_HPP

#include "dto/ArticleDto.hpp"

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iterator>

class TagCache {
  /**
    * This tag cache stores tag name and tag id into main memory
    * It has two std::map
    * one map stores tag name and pointer to tag id
    * another map stores tag id and pointer to tag name
    * Therefore, we can get tag name to id and tag id to tag name in O(1) with less memory usage
    */
  private:
    std::map<std::string, const std::string*> tagNamePairTagIdMap;
    std::map<std::string, const std::string*> tagIdPairTagNameMap;

  public:
    TagCache() {};
    void createTag(const std::pair<std::string, std::string> &tagNamePairTagId);
    std::string getIdFromName(const std::string &tagName);
    std::string getNameFromId(const std::string &id);
    bool nameExist(const std::string name); 
    const std::map<std::string, const std::string*>::iterator getTagNameFromIdBegin() {return tagNamePairTagIdMap.begin();};
    const std::map<std::string, const std::string*>::iterator getTagNameFromIdEnd() {return tagNamePairTagIdMap.end();};
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
    static void initCache();

    /**
    * This method is to create a list of tags.
    * @param tags The list of of tags.
    * @return bool This returns the result.
    */
    bool createTags(const oatpp::Vector<oatpp::String> &tags);

    /**
    * This method is to get all tags
    * @return oatpp::Object<TagJsonDto> This returns list of tags
    */
    oatpp::Object<TagJsonDto> getTags();

    /**
    * This method is to get a list of tag id from tags cache.
    * @param tags The list of tag name.
    * @return oatpp::Vector<oatpp::String> This returns the list of tag id.
    */
    oatpp::Vector<oatpp::String> getTagIdFromName(const oatpp::Vector<oatpp::String> &tags);

    /**
    * This method is to get a list of tag name from tags cache.
    * @param tagsId The list of tag id.
    * @return oatpp::Vector<oatpp::String> This returns the list of tag name.
    */
    oatpp::Vector<oatpp::String> getTagNameFromId(const std::vector<std::string> &tagsId);
};

#endif // TAGMODEL_HPP