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

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::RecordSet;
using Poco::Data::Statement;
using Poco::Exception;
using oatpp::web::protocol::http::Status;

oatpp::Object<TagJsonDto> TagModel::getTags() {
  try {
    Session session(Database::getPool()->get());
    Statement select(session);
    select << "SELECT name FROM tags ORDER BY name ASC", now;
    RecordSet rs(select);
    size_t rowCount = rs.totalRowCount();

    auto tags = TagJsonDto::createShared();
    tags->tags = {};
    tags->tags->resize(rowCount);
    for(int i = 0; i < rowCount; i++) {
      oatpp::String s = rs.value(0, i).toString();
      tags->tags->at(i) = s;
    }
     
    return tags;
  }
  catch(Exception& exp) {
    OATPP_LOGE("TagModel", exp.displayText().c_str());
    return nullptr;
  }
}