#include "model/TagModel.hpp"
#include "helper/Database.hpp"

#include "Poco/Data/Session.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"
#include "oatpp/core/base/Environment.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <iterator>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

std::map<std::string, std::string> TagModel::tagCache;

void TagModel::initCache() {
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT id, name FROM tags ORDER BY name ASC", now;
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    for(int i = 0; i < rowCount; i++) {
      tagCache.insert({rs.value(1, i).toString(), rs.value(0, i).toString()});
    }
  }
  catch(Exception& exp) {
    OATPP_LOGE("TagModel", exp.displayText().c_str());
  }
}

bool TagModel::createTags(std::vector<std::string> tags) {
  // Create tags if not exist
  auto it = tags.begin();
  while (it != tags.end()) {
    if(tagCache.find(*it) != tagCache.end()) {
      // Tag exist
      it = tags.erase(it);    
    }
    else {
      it++;
    }
  }

  if(!tags.empty()) {
    try {
      // Generate insert SQL
      Session session(Database::getPool()->get());
      Statement insert(session);
      insert << "INSERT INTO tags (name) VALUES ";
      for(int i = 0; i < tags.size(); i++) {
        insert << "(?)", use(tags[i]);
        if(i < (tags.size() - 1)) {
          insert << ",";
        }
      }
      insert.execute();
      
      // Get id for new tags
      Statement select(session);
      select << "SELECT id, name FROM tags WHERE name IN(";
      for(int i = 0; i < tags.size(); i++) {
        select << "?", use(tags[i]);
        if(i < (tags.size() - 1)) {
          select << ",";
        }
      }
      select << ")";
      select.execute();
      
      // Insert into cache
      RecordSet rs(select);
      size_t rowCount = rs.totalRowCount();
      for(int i = 0; i < rowCount; i++) {
        tagCache.insert({rs.value(1, i).toString(), rs.value(0, i).toString()});
      }
    }
    catch(Exception& exp) {
      OATPP_LOGE("TagModel", exp.displayText().c_str());
      return false;
    }
  }
  return true;
}

std::vector<std::string> TagModel::getTagsId(std::vector<std::string> tags) {
  std::vector<std::string> id(tags.size());
  for(int i = 0; i < tags.size(); i++) {
    id[i] = tagCache[tags[i]];
  }
  return id;
}

oatpp::Object<TagJsonDto> TagModel::getTags() {
  auto tags = TagJsonDto::createShared();
  tags->tags = {};
  tags->tags->resize(tagCache.size());

  auto it = tagCache.begin();
  int i = 0;
  while (it != tagCache.end())
  {
    std::string name = it->first;
    tags->tags->at(i++) = name;
    it++;
  }
  return tags;
}