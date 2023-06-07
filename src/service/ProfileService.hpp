#ifndef PROFILESERVICE_HPP
#define PROFILESERVICE_HPP

#include "model/UserHasFollowerModel.hpp"
#include "model/UserModel.hpp"

#include "oatpp/web/protocol/http/Http.hpp"

class ProfileService {
private:
  UserHasFollowerModel userHasFollowerModel;
  UserModel userModel;
  typedef oatpp::web::protocol::http::Status Status;

public:
  oatpp::Object<UserProfileJsonDto> getProfile(std::string &id, std::string &profileUsername);
  oatpp::Object<UserProfileJsonDto> followProfile(std::string &id, std::string &profileUsername);
  oatpp::Object<UserProfileJsonDto> unfollowProfile(std::string &id, std::string &profileUsername);
};

#endif // PROFILESERVICE_HPP