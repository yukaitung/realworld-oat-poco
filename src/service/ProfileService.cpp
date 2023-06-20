#include "service/ProfileService.hpp"

oatpp::Object<UserProfileJsonDto> ProfileService::getProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_422, "The username is missing.");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Internal Server Error.");
  OATPP_ASSERT_HTTP(!profile.second.empty(), Status::CODE_404, "The profile could not be found.");
  if(!id.empty()) {
    // See if the user is following the profile
    std::vector<std::string> following = {profile.second};
    profile.first->following = userHasFollowerModel.validUserFollowing(id, following).size() == 1;
  }

  auto response = UserProfileJsonDto::createShared();
  response->profile = profile.first;
  return response;
}

oatpp::Object<UserProfileJsonDto> ProfileService::followProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_422, "The username is missing.");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Internal Server Error.");
  OATPP_ASSERT_HTTP(!profile.second.empty(), Status::CODE_404, "The profile could not be found.");

  bool result = userHasFollowerModel.userNewFollower(profile.second, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  auto response = UserProfileJsonDto::createShared();
  profile.first->following = true;
  response->profile = profile.first;
  return response;
}

oatpp::Object<UserProfileJsonDto> ProfileService::unfollowProfile(std::string &id, std::string &profileUsername) {
  OATPP_ASSERT_HTTP(!profileUsername.empty(), Status::CODE_422, "The username is missing.");

  auto profile = userModel.getProfileFromUsername(profileUsername);
  OATPP_ASSERT_HTTP(profile.first != nullptr, Status::CODE_500, "Internal Server Error.");
  OATPP_ASSERT_HTTP(!profile.second.empty(), Status::CODE_404, "The profile could not be found.");

  bool result = userHasFollowerModel.userRemoveFollower(profile.second, id);
  OATPP_ASSERT_HTTP(result, Status::CODE_500, "Internal Server Error.");

  auto response = UserProfileJsonDto::createShared();
  profile.first->following = false;
  response->profile = profile.first;
  return response;
}