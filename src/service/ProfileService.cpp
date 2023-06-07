#include "service/ProfileService.hpp"

oatpp::Object<UserProfileJsonDto> ProfileService::getProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_400, "Missing username");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Server Error");
  if(!id.empty()) {
    // See if the user is following the profile
    profile.first->following = userHasFollowerModel.userHasThisFollower(profile.second, id);
  }

  auto response = UserProfileJsonDto::createShared();
  response->profile = profile.first;
  return response;
}

oatpp::Object<UserProfileJsonDto> ProfileService::followProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_400, "Missing username");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Server Error");

  bool result = userHasFollowerModel.userNewFollower(profile.second, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server Error");

  auto response = UserProfileJsonDto::createShared();
  profile.first->following = true;
  response->profile = profile.first;
  return response;
}

oatpp::Object<UserProfileJsonDto> ProfileService::unfollowProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_400, "Missing username");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Server Error");

  bool result = userHasFollowerModel.userRemoveFollower(profile.second, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Server Error");

  auto response = UserProfileJsonDto::createShared();
  profile.first->following = false;
  response->profile = profile.first;
  return response;
}